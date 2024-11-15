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

#include "cbase.h"
#include "CBaseItem.h"

BEGIN_DATAMAP(CBaseItem)
	DEFINE_FIELD(m_RespawnDelay, FIELD_FLOAT),
	DEFINE_FIELD(m_IsRespawning, FIELD_BOOLEAN),
	DEFINE_FIELD(m_PlayPickupSound, FIELD_BOOLEAN),
	DEFINE_FIELD(m_TriggerOnSpawn, FIELD_STRING),
	DEFINE_FIELD(m_TriggerOnDespawn, FIELD_STRING),
	DEFINE_FUNCTION(FallThink),
	DEFINE_FUNCTION(ItemTouch),
	DEFINE_FUNCTION(Materialize),
	DEFINE_FUNCTION(AttemptToMaterialize),
END_DATAMAP();

bool CBaseItem::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "respawn_delay"))
	{
		m_RespawnDelay = atof(pkvd->szValue);

		if (m_RespawnDelay < 0)
		{
			m_RespawnDelay = ITEM_NEVER_RESPAWN_DELAY;
		}

		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "trigger_on_spawn"))
	{
		m_TriggerOnSpawn = ALLOC_STRING(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "trigger_on_despawn"))
	{
		m_TriggerOnDespawn = ALLOC_STRING(pkvd->szValue);
		return true;
	}

	return CBaseAnimating::KeyValue(pkvd);
}

void CBaseItem::SetupItem(const Vector& mins, const Vector& maxs)
{
	pev->movetype = MOVETYPE_TOSS;

	pev->solid = SOLID_TRIGGER;

	SetSize(mins, maxs);
	SetOrigin(pev->origin);

	SetTouch(&CBaseItem::ItemTouch);

	SetThink(&CBaseItem::FallThink);

	pev->nextthink = gpGlobals->time + 0.1;

	// All items are animated by default
	pev->framerate = 1;
}

void CBaseItem::Precache()
{
	if (auto modelName = GetModelName(); *modelName)
	{
		PrecacheModel(modelName);
	}

	PrecacheSound("items/weapondrop1.wav"); // item falls to the ground
	PrecacheSound("items/suitchargeok1.wav");
}

void CBaseItem::Spawn()
{
#ifndef CLIENT_DLL
	Precache();

	if (FStrEq(GetModelName(), ""))
	{
		Logger->warn("{}:{} has no model", GetClassname(), entindex());
	}

	SetModel(GetModelName());
	SetupItem({-16, -16, 0}, {16, 16, 16});
#endif
}

void CBaseItem::FallThink()
{
#ifndef CLIENT_DLL

	if ((pev->flags & FL_ONGROUND) != 0)
	{
		// clatter if we have an owner (i.e., dropped by someone)
		// don't clatter if the item is waiting to respawn (if it's waiting, it is invisible!)
		if (!FNullEnt(GetOwner()))
		{
			EmitSoundDyn(CHAN_VOICE, "items/weapondrop1.wav", VOL_NORM, ATTN_NORM, 0, 95 + RANDOM_LONG(0, 29));
		}

		// lie flat
		pev->angles.x = 0;
		pev->angles.z = 0;

		Materialize();
	}
	else
	{
		pev->nextthink = gpGlobals->time + 0.1;
	}
#endif
}

void CBaseItem::ItemTouch(CBaseEntity* pOther)
{
#ifndef CLIENT_DLL
	// if it's not a npc or player, ignore
	if( auto monster = dynamic_cast<CBaseMonster*>(pOther); monster )
	{
		InventoryAddItem(monster);
	}
#endif
}

ItemAddResult CBaseItem::InventoryAddItem(CBaseMonster* monster)
{
#ifndef CLIENT_DLL

	// Don't add item to dead mobs
	if( !monster->IsAlive() )
	{
		return ItemAddResult::NotAdded;
	}

	if( auto player = ToBasePlayer(monster->edict()); player != nullptr )
	{
		// Don't add items to observer players
		if( player->IsObserver() )
		{
			return ItemAddResult::NotAdded;
		}
	}

	// Only add items to valid inventory slots
	auto IsValidSlot = []( size_t index, const char* classname ) -> bool
	{
		InventorySlot Try = static_cast<InventorySlot>(index);

		if( Try == InventorySlot::LeftHand )
			return false;
		if( Try >= InventorySlot::ArmorHelmet && Try <= InventorySlot::ArmorBoots )
			return false;
		if( Try == InventorySlot::Arrows && !strstr( classname, "minecraft_arrow" ) )
			return false;

		return true;
	};

	// We can only carry one item of this. find a empty slot
	if( max_items == 1 )
	{
		for( size_t i = 0; i < monster->inventory.size(); ++i )
		{
			if( !IsValidSlot(i, GetClassname()) )
				continue;

			if( auto pItem = monster->inventory.at(i); !pItem || pItem == nullptr )
			{
				monster->inventory.at(i) = this;
				return ItemAddResult::Added;
			}
		}
		return ItemAddResult::NotAdded;
	}

	InventorySlot EmptySlot = InventorySlot::InvalidAddSlot;

	bool SomeFilled = false;

	// Find stackable items and track a empty slot
    for( size_t i = 0; i < monster->inventory.size(); ++i )
	{
		if( !IsValidSlot(i, GetClassname()) )
			continue;

		if( auto pItem = monster->inventory.at(i); pItem != nullptr )
		{
			// Fill items if they're the same.
			if( FStrEq( pItem->GetClassname(), GetClassname() ) )
			{
				if( pItem->items < pItem->max_items )
				{
					int diff = std::min( items, pItem->max_items - pItem->items );
					pItem->items += diff;
					items -= diff;

					// All filled!
					if( items <= 0 ) {
						return ItemAddResult::Filled;
					}

					SomeFilled = true;
				}
			}
		}
		else if( EmptySlot == InventorySlot::InvalidAddSlot )
		{
			EmptySlot = static_cast<InventorySlot>(i);
		}
	}

	if( EmptySlot != InventorySlot::InvalidAddSlot )
	{
		monster->inventory.at((int)EmptySlot) = this;
		return ItemAddResult::Added;
	}

	if( SomeFilled )
	{
		return ItemAddResult::NotAllAdded;
	}

#endif

	return ItemAddResult::NotAdded;
}

CBaseItem* CBaseItem::GetItemToRespawn(const Vector& respawnPoint)
{
#ifndef CLIENT_DLL
	SetOrigin(respawnPoint); // move to wherever I'm supposed to respawn.
#endif

	return this;
}

CBaseItem* CBaseItem::Respawn()
{
#ifndef CLIENT_DLL
	if (auto newItem = GetItemToRespawn(g_pGameRules->ItemRespawnSpot(this)); newItem)
	{
		// not a typo! We want to know when the item the player just picked up should respawn! This new entity we created is the replacement,
		// but when it should respawn is based on conditions belonging to the item that was taken.
		const float respawnTime = g_pGameRules->ItemRespawnTime(this);

		newItem->m_IsRespawning = true;

		newItem->SetTouch(nullptr);
		newItem->SetThink(&CBaseItem::AttemptToMaterialize);

		newItem->pev->nextthink = respawnTime;

		return newItem;
	}

	Logger->error("Item respawn failed to create {}!", GetClassname());
#endif

	return nullptr;
}

void CBaseItem::Materialize()
{
#ifndef CLIENT_DLL
	if (m_IsRespawning)
	{
		// changing from invisible state to visible.
		EmitSoundDyn(CHAN_WEAPON, "items/suitchargeok1.wav", VOL_NORM, ATTN_NORM, 0, 150);

		pev->effects &= ~EF_NODRAW;

		m_IsRespawning = false;
	}

	pev->solid = SOLID_TRIGGER;
	SetOrigin(pev->origin); // link into world.
	SetTouch(&CBaseItem::ItemTouch);
	SetThink(nullptr);

	if (!FStringNull(m_TriggerOnSpawn))
	{
		FireTargets(STRING(m_TriggerOnSpawn), this, this, USE_ON, 0);
	}
#endif
}

void CBaseItem::AttemptToMaterialize()
{
#ifndef CLIENT_DLL
	const float time = g_pGameRules->ItemTryRespawn(this);

	if (time == 0)
	{
		pev->flags &= ~FL_ONGROUND;
		SetThink(&CBaseItem::FallThink);
		pev->nextthink = gpGlobals->time + 0.1;
		return;
	}

	pev->nextthink = time;
#endif
}
