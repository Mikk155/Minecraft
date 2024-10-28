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
	static constexpr char GameModeName[] = "minecraft";

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
};

namespace minecraft
{
	const int FUnbreakable -100

	constexpr std::string_view arthropods[] = {
		"monster_spider",
		"monster_cave_spider",
		"monster_silverfish"
	};

	constexpr std::string_view undead[] = {
		"monster_zombie",
		"monster_zombie_villager",
		"monster_zombie_husk",
		"monster_zombie_drowned",
		"monster_zombie_piglin",
		"monster_zombie_horse",
		"monster_skeleton",
		"monster_skeleton_wither",
		"monster_skeleton_stray",
		"monster_skeleton_horse",
		"monster_whither_boss",
		"monster_phantom"
	};

	namespace effect
	{
		static constexpr std::string_view roman_numbs_literal[] = { "X", "IX", "V", "IV", "I" };

		static constexpr int roman_numbs_integers[] = { 10, 9, 5, 4, 1  };

		/**
		*	@brief Converts the given integer to the roman value
		*
		*	A leading whitespace will be added to the string
		*	@param num if 0 returns empty string
		*/
		std::string level( int num );

		struct data
		{
			int max_level;
			std::vector<std::string_view> incompatibilities;
		}

		/**
		*	@brief Sharpness adds 0.5 * level + 0.5 extra damage.
		*/
		std::string_view sharpness = "Sharpness"sv;

		/**
		*	@brief Each level on a sword or axe adds 2.5 ♥ × level of extra damage to each hit on arthropods only.
		*
		*	The enchantment also inflicts Slowness IV on an arthropod when hit, with a random duration between 1 and 1.5 seconds at level I increasing the maximum duration by 0.5 seconds at each level, up to 3.5 seconds with Bane of Arthropods V.
		*/
		std::string_view bane_of_arthropods = "Bane of Arthropods"sv;

		/**
		*	@brief Smite increases damage dealt to un-dead mobs.
		*
		*	Each level separately adds 2.5 ♥ × level of extra damage on each hit to undead mobs.
		*/
		std::string_view smite = "Smite"sv;

		/**
		*	@brief Fire Aspect adds 4 seconds of burning per level to the target.
		*
		*	Because the first hit is caused by the item with this enchantment, the first second of fire damage is not recognized. Targets receive 3 ♥ and 6 ♥ damage with level I and II respectively.
		*/
		std::string_view fire_aspect = "Fire Ascpect"sv;

		/**
		*	@brief Knock Back will push the hit entity based on the received damage and enchant level.
		*/
		std::string_view knockback = "Knock Back"sv;

		/**
		*	@brief Life Steal restores health to the attacker in 10% per level of the dealt damage.
		*
		*	This is done before any damage multiplication and after damage deductions.
		*/
		std::string_view life_steal = "Life Steal"sv;

		/**
		*	@brief Critic Chance has a chance of 10% per level to multiply the damage dealth by a random value between 1.0 and 2.0.
		*
		*	This is done before any damage multiplication and after damage deductions.
		*/
		std::string_view critical_chance = "Critic Chance"sv;

		std::unordered_map<std::string_view, data> enchantment = {

			{ sharpness, data( 5, { bane_of_arthropods, smite } ) },
			{ bane_of_arthropods, data( 5, { sharpness, smite } ) },
			{ smite, data( 5, { sharpness, bane_of_arthropods } ) },

			{ fire_aspect, data( 2, { life_steal, critical_chance } ) },
			{ life_steal, data( 5, { fire_aspect, critical_chance } ) },
			{ critical_chance, data( 7, { fire_aspect, life_steal } ) },

			{ knockback, data( 2, {} ) }
		};
	}
}
