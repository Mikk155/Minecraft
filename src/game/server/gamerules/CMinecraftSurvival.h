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

#include <memory>

#include "CHalfLifeMultiplay.h"

class CMinecraftSurvival : public CHalfLifeMultiplay
{
public:
	static constexpr char GameModeName[] = "mcsurvival";

	CMinecraftSurvival();

	const char* GetGameModeName() const override { return GameModeName; }

	bool IsCoOp() override { return true; }
	bool IsTeamplay() override { return true; }

	void UpdateGameMode(CBasePlayer* pPlayer) override;

	void DeathNotice(CBasePlayer* pVictim, CBaseEntity* pKiller, CBaseEntity* inflictor) override {}

	void MonsterKilled(CBaseMonster* pVictim, CBaseEntity* pKiller, CBaseEntity* inflictor) override;

	int PlayerRelationship(CBasePlayer* pPlayer, CBaseEntity* pTarget) override;

	bool ShouldAutoAim(CBasePlayer* pPlayer, CBaseEntity* target) override;

	int IPointsForKill(CBasePlayer* pAttacker, CBasePlayer* pKilled) override;

	int IPointsForMonsterKill(CBasePlayer* pAttacker, CBaseMonster* pKilled) override;

	bool FPlayerCanRespawn(CBasePlayer* pPlayer) override { return true; }

	float FlPlayerSpawnTime(CBasePlayer* pPlayer) override;

	int DeadPlayerWeapons(CBasePlayer* pPlayer) override;

	void PlayerKilled(CBasePlayer* pVictim, CBaseEntity* pKiller, CBaseEntity* inflictor) override;

	void Think() override;

	bool FPlayerCanTakeDamage(CBasePlayer* pPlayer, CBaseEntity* pAttacker) override;

	const char* GetGameDescription() override { return "Minecraft Survival"; }

private:
	bool m_DisableDeathMessages = false;
	bool m_DisableDeathPenalty = false;

	ScopedClientCommand m_MenuSelectCommand;
};

namespace Minecraft
{
	struct EffectsTable
	{
		const char* Mending = "Mending";
		const char* Unbreaking = "Unbreaking";
		const char* CurseofVanishing = "Curse of Vanishing";
		const char* AquaAffinity = "Aqua Affinity";
		const char* BlastProtection = "Blast Protection";
		const char* CurseofBinding = "Curse of Binding";
		const char* DepthStrider = "Depth Strider";
		const char* FeatherFalling = "Feather Falling";
		const char* FireProtection = "Fire Protection";
		const char* FrostWalker = "Frost Walker";
		const char* ProjectileProtection = "Projectile Protection";
		const char* Protection = "Protection";
		const char* Respiration = "Respiration";
		const char* SoulSpeed = "Soul Speed";
		const char* Thorns = "Thorns";
		const char* SwiftSneak = "Swift Sneak";
		const char* Channeling = "Channeling";
		const char* Flame = "Flame";
		const char* Impaling = "Impaling";
		const char* Infinity = "Infinity";
		const char* Loyalty = "Loyalty";
		const char* Riptide = "Riptide";
		const char* Multishot = "Multishot";
		const char* Piercing = "Piercing";
		const char* Power = "Power";
		const char* Punch = "Punch";
		const char* QuickCharge = "Quick Charge";
		const char* SweepingEdge = "Sweeping Edge";
		const char* Smite = "Smite";
		const char* Sharpness = "Sharpness";
		const char* Knockback = "Knockback";
		const char* Looting = "Looting";
		const char* FireAspect = "Fire Aspect";
		const char* BaneofArthropods = "Bane of Arthropods";
		const char* Density = "Density";
		const char* Breach = "Breach";
		const char* WindBurst = "Wind Burst";
	};

	struct Effects
	{
		// < EffectsTable, int value >
		// i.e ``"Sharpness", 1``
		std::unordered_map<std::string_view, int> effect;
	};
}