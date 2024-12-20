/***
 *
 *	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/

#include <algorithm>
#include <string>
#include <vector>

#include "cbase.h"
#include "GameLibrary.h"
#include "CGameRules.h"
#include "CHalfLifeCoopplay.h"
#include "CHalfLifeCTFplay.h"
#include "CHalfLifeMultiplay.h"
#include "CHalfLifeRules.h"
#include "CHalfLifeTeamplay.h"
#include "PersistentInventorySystem.h"
#include "SpawnInventorySystem.h"
#include "spawnpoints.h"
#include "UserMessages.h"
#include "items/weapons/AmmoTypeSystem.h"

class ItemRespawnTimeVisitor final : public IItemVisitor
{
public:

	void Visit(CBasePlayerWeapon* weapon) override
	{

		if (RespawnTime != ITEM_NEVER_RESPAWN_DELAY )
		{
			// make sure it's only certain weapons
			if ((weapon->iFlags() & ITEM_FLAG_LIMITINWORLD) == 0)
			{
				RespawnTime = 0; // weapon respawns almost instantly
				return;
			}
		}
	}

	void Visit(CItem* pickupItem) override
	{
		RespawnTime = ITEM_NEVER_RESPAWN_DELAY;
	}

	// Don't respawn unknown items.
	float RespawnTime = ITEM_NEVER_RESPAWN_DELAY;
};

class ItemTryRespawnVisitor final : public IItemVisitor
{
public:
	explicit ItemTryRespawnVisitor(CGameRules* gameRules)
		: GameRules(gameRules)
	{
	}

	void Visit(CBasePlayerWeapon* weapon) override
	{
		if (WEAPON_NONE != weapon->m_iId && (weapon->iFlags() & ITEM_FLAG_LIMITINWORLD) != 0)
		{
			if (NUMBER_OF_ENTITIES() < (gpGlobals->maxEntities - ENTITY_INTOLERANCE))
				return;

			// we're past the entity tolerance level, so delay the respawn
			DelayRespawn = true;
		}
	}

	void Visit(CItem* pickupItem) override
	{
	}

	CGameRules* const GameRules;
	bool DelayRespawn = false;
};

class GameRulesCanHaveItemVisitor : public IItemVisitor
{
public:
	explicit GameRulesCanHaveItemVisitor(CGameRules* gameRules, CBasePlayer* player)
		: GameRules(gameRules),
		  Player(player)
	{
	}

	void Visit(CBasePlayerWeapon* weapon) override
	{
		// only living players can have items
		if (Player->pev->deadflag != DEAD_NO)
		{
			CanHaveItem = false;
			return;
		}

		if (weapon->pszAmmo1())
		{
			if (!GameRules->CanHaveAmmo(Player, weapon->pszAmmo1()))
			{
				// we can't carry anymore ammo for this gun. We can only
				// have the gun if we aren't already carrying one of this type
				if (Player->HasPlayerWeapon(weapon))
				{
					CanHaveItem = false;
					return;
				}
			}
		}
		else
		{
			// weapon doesn't use ammo, don't take another if you already have it.
			if (Player->HasPlayerWeapon(weapon))
			{
				CanHaveItem = false;
				return;
			}
		}

		// note: will fall through to here if GetItemInfo doesn't fill the struct!
	}

	void Visit(CItem* pickupItem) override {}

	CGameRules* const GameRules;
	CBasePlayer* const Player;
	bool CanHaveItem = true;
};

CGameRules::CGameRules()
{
	m_SpectateCommand = g_ClientCommands.CreateScoped("spectate", [this](CBasePlayer* player, const auto& args)
		{
			// clients wants to become a spectator
			BecomeSpectator(player, args); });

	m_SpecModeCommand = g_ClientCommands.CreateScoped("specmode", [this](CBasePlayer* player, const auto& args)
		{
			// new spectator mode
			if (player->IsObserver())
				player->Observer_SetMode(atoi(CMD_ARGV(1))); });
}

void CGameRules::SetupPlayerInventory(CBasePlayer* player)
{
	// Originally game_player_equip entities were triggered in PlayerSpawn to set up the player's inventory.
	// This is now handled by naming them game_playerspawn (see CBasePlayer::UpdateClientData).
	// Handling it there avoids edge cases where this function is called during ClientPutInServer.
	// It is not possible to send messages to clients during that function so ammo change messages are ignored.

	if (!g_PersistentInventory.TryApplyToPlayer(player))
	{
		g_SpawnInventory.GetInventory()->ApplyToPlayer(player);
	}
}

float CGameRules::GetRespawnDelay(CBaseItem* item)
{
	if (item->m_RespawnDelay == ITEM_NEVER_RESPAWN_DELAY)
	{
		return ITEM_NEVER_RESPAWN_DELAY;
	}

	if (item->m_RespawnDelay != ITEM_DEFAULT_RESPAWN_DELAY)
	{
		return item->m_RespawnDelay;
	}

	ItemRespawnTimeVisitor visitor;
	item->Accept(visitor);
	return visitor.RespawnTime;
}

bool CGameRules::ItemShouldRespawn(CBaseItem* item)
{
	const float delay = GetRespawnDelay(item);

	return delay != ITEM_NEVER_RESPAWN_DELAY;
}

float CGameRules::ItemRespawnTime(CBaseItem* item)
{
	const float delay = GetRespawnDelay(item);

	if (delay == ITEM_NEVER_RESPAWN_DELAY)
	{
		return ITEM_NEVER_RESPAWN_DELAY;
	}

	return gpGlobals->time + delay;
}

Vector CGameRules::ItemRespawnSpot(CBaseItem* item)
{
	return item->pev->origin;
}

float CGameRules::ItemTryRespawn(CBaseItem* item)
{
	ItemTryRespawnVisitor visitor{this};
	item->Accept(visitor);

	if (visitor.DelayRespawn)
	{
		const float delay = GetRespawnDelay(item);

		// This function should only be called if the item has returned a valid delay from ItemRespawnTime.
		assert(delay >= 0);

		return delay;
	}

	return 0;
}

bool CGameRules::CanHaveAmmo(CBasePlayer* pPlayer, const char* pszAmmoName)
{
	if (pszAmmoName)
	{
		if (const auto type = g_AmmoTypes.GetByName(pszAmmoName); type)
		{
			if (pPlayer->GetAmmoCountByIndex(type->Id) < type->MaximumCapacity)
			{
				// player has room for more of this type of ammo
				return true;
			}
		}
	}

	return false;
}

void CGameRules::PlayerSpawn(CBasePlayer* pPlayer)
{
	SetupPlayerInventory(pPlayer);
	pPlayer->m_FireSpawnTarget = true;
}

CBaseEntity* CGameRules::GetPlayerSpawnSpot(CBasePlayer* pPlayer)
{
	CBaseEntity* pSpawnSpot = EntSelectSpawnPoint(pPlayer);

	pPlayer->pev->origin = pSpawnSpot->pev->origin + Vector(0, 0, 1);
	pPlayer->pev->v_angle = g_vecZero;
	pPlayer->pev->velocity = g_vecZero;
	pPlayer->pev->angles = pSpawnSpot->pev->angles;
	pPlayer->pev->punchangle = g_vecZero;
	pPlayer->pev->fixangle = FIXANGLE_ABSOLUTE;

	return pSpawnSpot;
}

void CGameRules::ClientUserInfoChanged(CBasePlayer* pPlayer, char* infobuffer)
{
	pPlayer->SetPrefsFromUserinfo(infobuffer);
}

bool CGameRules::CanHaveItem(CBasePlayer* player, CBaseItem* item)
{
	GameRulesCanHaveItemVisitor visitor{this, player};
	item->Accept(visitor);
	return visitor.CanHaveItem;
}

void CGameRules::BecomeSpectator(CBasePlayer* player, const CommandArgs& args)
{
	// Default implementation: applies to all game modes, even singleplayer.

	// always allow proxies to become a spectator
	if ((player->pev->flags & FL_PROXY) != 0 || allow_spectators.value != 0)
	{
		CBaseEntity* pSpawnSpot = GetPlayerSpawnSpot(player);
		player->StartObserver(player->pev->origin, pSpawnSpot->pev->angles);

		// notify other clients of player switching to spectator mode
		UTIL_ClientPrintAll(HUD_PRINTNOTIFY, UTIL_VarArgs("%s switched to spectator mode\n",
												 (!FStringNull(player->pev->netname) && STRING(player->pev->netname)[0] != 0) ? STRING(player->pev->netname) : "unconnected"));
	}
	else
		ClientPrint(player, HUD_PRINTCONSOLE, "Spectator mode is disabled.\n");
}

template <typename TGameRules>
CGameRules* CreateGameRules()
{
	return new TGameRules();
}

using GameRulesFactory = CGameRules* (*)();

using GameRulesEntry = std::pair<const char*, GameRulesFactory>;

template <typename TGameRules>
GameRulesEntry CreateGameRulesEntry()
{
	return {TGameRules::GameModeName, &CreateGameRules<TGameRules>};
}

// Map of all multiplayer gamerules.
static const std::vector<GameRulesEntry> GameRulesList{
	CreateGameRulesEntry<CHalfLifeMultiplay>(),
	CreateGameRulesEntry<CHalfLifeTeamplay>(),
	CreateGameRulesEntry<CHalfLifeCTFplay>(),
	CreateGameRulesEntry<CHalfLifeCoopplay>()};

CGameRules* InstallGameRules(std::string_view gameModeName)
{
	CGameRules::Logger->trace("Creating gamerules");

	if (gpGlobals->maxClients == 1)
	{
		if (!gameModeName.empty())
		{
			CGameRules::Logger->info("Ignoring gamemode setting {} in singleplayer", gameModeName);
		}

		CGameRules::Logger->trace("Creating singleplayer gamerules");
		// generic half-life
		return CreateGameRules<CHalfLifeRules>();
	}

	if (!gameModeName.empty())
	{
		if (auto it = std::find_if(GameRulesList.begin(), GameRulesList.end(), [&](const auto& candidate)
				{ return candidate.first == gameModeName; });
			it != GameRulesList.end())
		{
			CGameRules::Logger->trace("Creating {} gamerules", gameModeName);
			return it->second();
		}

		CGameRules::Logger->error("Couldn't create {} gamerules, falling back to deathmatch game mode", gameModeName);
	}
	else
	{
		CGameRules::Logger->trace("Autodetected deathmatch game mode");
	}

	CGameRules::Logger->trace("Creating deathmatch gamerules");

	return CreateGameRules<CHalfLifeMultiplay>();
}

const char* GameModeIndexToString(int index)
{
	// Autodetect game mode.
	if (index == 0)
	{
		return "";
	}

	--index;

	if (index < 0 || static_cast<std::size_t>(index) >= GameRulesList.size())
	{
		CGameRules::Logger->error("Invalid mp_createserver_gamemode value");
		return "";
	}

	return GameRulesList[index].first;
}

void PrintMultiplayerGameModes()
{
	Con_Printf("Singleplayer always uses singleplayer gamerules\n");
	Con_Printf("Set mp_gamemode to \"autodetect\" to autodetect the game mode\n");

	Con_Printf("%s (not an option for mp_gamemode)\n", CHalfLifeRules::GameModeName);

	for (const auto& gameMode : GameRulesList)
	{
		Con_Printf("%s\n", gameMode.first);
	}
}
