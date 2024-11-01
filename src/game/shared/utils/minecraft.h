#ifndef MINECRAFT_H
#define MINECRAFT_H

#include <unordered_map>
#include <vector>
#include <string_view>

#include "entities/cbase.h"

#pragma once

namespace minecraft
{
	namespace slot
	{

		const int LEFT_HAND = 0;
		const int SLOT1 = 1;
		const int SLOT2 = 2;
		const int SLOT3 = 3;
		const int SLOT4 = 4;
		const int SLOT5 = 5;
		const int SLOT6 = 6;
		const int SLOT7 = 7;
		const int SLOT8 = 8;
		const int SLOT9 = 9;
		const int HELMET = 10;
		const int CHEST = 11;
		const int LEGS = 12;
		const int BOOTS = 13;
		const int ARROWS = 14;
		//
		const int LAST_SLOT = 40;
	}

	struct inventory
	{
#ifndef CLIENT_DLL
		CBaseEntity* pItem;
		int max_stack;
		int amount;

		inventory( CBaseEntity* pItem = nullptr, int max_stack = 64, int amount = 1 )
			: pItem(pItem), max_stack(max_stack), amount(amount) {}
#endif
	};

	struct effects
	{
#ifndef CLIENT_DLL
		// Effect name
		std::string_view name;
		// Effect level
		int level = 1;
		// Time till the effect ends
		float end;
		// Time, used for comparitions in Think, is the cooldown till apply the effect
		float time;
		// Last time, used in gamerules for comparing with the above float
		float last_time = 0;
		// Should we send the info to the client? This is managed entirely on gamerules.
		bool should_update = true;

		// These are for wither, poison, fire etc
		CBaseEntity* inflictor = nullptr;
		CBaseEntity* attacker = nullptr;

		effects( std::string_view name = nullptr, float level = 1, float end = 1.0f, float time = 0.0f, CBaseEntity* inflictor = nullptr, CBaseEntity* attacker = nullptr )
			: name(name), level(level), end(end), time(time), inflictor(inflictor), attacker(attacker) {}
#endif
	};

	const int FUnbreakable = -100;

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
		"monster_wither_boss",
		"monster_phantom"
	};

	namespace effect
	{
		struct data_enchant
		{
			int max_level;
			std::vector<std::string_view> incompatibilities;

			data_enchant( int max_level, std::vector<std::string_view> incompatibilities )
				: max_level( max_level ), incompatibilities( std::move( incompatibilities ) ) {}
		};

		/**
		*	@brief Sharpness adds 0.5 * level + 0.5 extra damage.
		*/
		constexpr std::string_view sharpness = "Sharpness";

		/**
		*	@brief Each level on a sword or axe adds 2.5 ♥ × level of extra damage to each hit on arthropods only.
		*
		*	The enchantment also inflicts Slowness IV on an arthropod when hit, with a random duration between 1 and 1.5 seconds at level I increasing the maximum duration by 0.5 seconds at each level, up to 3.5 seconds with Bane of Arthropods V.
		*/
		constexpr std::string_view bane_of_arthropods = "Bane of Arthropods";

		/**
		*	@brief Smite increases damage dealt to un-dead mobs.
		*
		*	Each level separately adds 2.5 ♥ × level of extra damage on each hit to undead mobs.
		*/
		constexpr std::string_view smite = "Smite";

		/**
		*	@brief Fire Aspect adds 4 seconds of burning per level to the target.
		*
		*	Because the first hit is caused by the item with this enchantment, the first second of fire damage is not recognized. Targets receive 3 ♥ and 6 ♥ damage with level I and II respectively.
		*/
		constexpr std::string_view fire_aspect = "Fire Ascpect";

		/**
		*	@brief Life Steal restores health to the attacker in 10% per level of the dealt damage.
		*
		*	This is done before any damage multiplication and after damage deductions.
		*/
		constexpr std::string_view life_steal = "Life Steal";

		/**
		*	@brief Critic Chance has a chance of 10% per level to multiply the damage dealth by a random value between 1.0 and 2.0.
		*
		*	This is done before any damage multiplication and after damage deductions.
		*/
		constexpr std::string_view critical_chance = "Critic Chance";

		/**
		*	@brief Knock Back will push the hit entity based on the received damage and enchant level.
		*/
		constexpr std::string_view knockback = "Knock Back";

		/**
		*	@brief Sweeping Edge Expanding to 512 units from the victim and hurt enemies in the path.
        *
        *   Does 25% (per level) of the original damage on each entity slashed. the formula is done after both the entity and the weapon buffs.
		* 
		*	This uses DMG_SLASH to prevent recursion, use DMG_SLASH safely.
		*/
		constexpr std::string_view sweeping_edge = "Sweeping Edge";

		std::unordered_map<std::string_view, data_enchant> enchantment = {

			{ sharpness, data_enchant( 5, { bane_of_arthropods, smite } ) },
			{ bane_of_arthropods, data_enchant( 5, { sharpness, smite } ) },
			{ smite, data_enchant( 5, { sharpness, bane_of_arthropods } ) },

			{ fire_aspect, data_enchant( 2, { life_steal, critical_chance } ) },
			{ life_steal, data_enchant( 5, { fire_aspect, critical_chance } ) },
			{ critical_chance, data_enchant( 7, { fire_aspect, life_steal } ) },

			{ knockback, data_enchant( 2, { sweeping_edge } ) },
			{ sweeping_edge, data_enchant( 3, { knockback } ) }
		};

		/**
		*	@brief Fire burns the victim and apply 0.5 ♥ every second for 4 seconds,
        *
        *   When level 2 the seconds expands to 8.
		* 
		*	When level 3 it apply every half of a second
		*/
		constexpr std::string_view fire = "Fire";

		/**
		*	@brief Speed is a status effect that increases an entity's walking speed by 20% multiplied by the effect level. It expands a player's field of view (FOV)
		*/
		constexpr std::string_view speed = "Speed";

		/**
		*	@brief Slowness decreases walking speed by 15% × level and contracts the player's field of view accordingly.
		*/
		constexpr std::string_view slowness = "Slowness";

		/**
		*	@brief Fire burns the victim and apply 0.5 ♥ every second for 4 seconds,
        *
        *   When level 2 the seconds expands to 8.
		* 
		*	When level 3 it apply every half of a second
		*/
		constexpr std::string_view fire_resistance = "Fire Resistance";

		std::unordered_map<std::string_view, int> effects = {

			{ fire, 3 },
			{ speed, 2 },
			{ fire_resistance, 0 },
			{ slowness, 4 }
		};
		/*
			https://minecraft.fandom.com/wiki/Haste
			https://minecraft.fandom.com/wiki/Mining_Fatigue
			https://minecraft.fandom.com/wiki/Strength
			https://minecraft.fandom.com/wiki/Instant_Health
			https://minecraft.fandom.com/wiki/Instant_Damage
			https://minecraft.fandom.com/wiki/Jump_Boost
			https://minecraft.fandom.com/wiki/Nausea
			https://minecraft.fandom.com/wiki/Regeneration
			https://minecraft.fandom.com/wiki/Resistance
			https://minecraft.fandom.com/wiki/Fire_Resistance
			https://minecraft.fandom.com/wiki/Water_Breathing
			https://minecraft.fandom.com/wiki/Invisibility
			https://minecraft.fandom.com/wiki/Blindness
			https://minecraft.fandom.com/wiki/Night_Vision
			https://minecraft.fandom.com/wiki/Hunger_(effect)
			https://minecraft.fandom.com/wiki/Weakness
			https://minecraft.fandom.com/wiki/Poison
			https://minecraft.fandom.com/wiki/Wither_(effect)
			https://minecraft.fandom.com/wiki/Health_Boost
			https://minecraft.fandom.com/wiki/Absorption
			https://minecraft.fandom.com/wiki/Saturation
			https://minecraft.fandom.com/wiki/Glowing
			https://minecraft.fandom.com/wiki/Levitation
			https://minecraft.fandom.com/wiki/Luck
			https://minecraft.fandom.com/wiki/Bad_Luck
			https://minecraft.fandom.com/wiki/Fatal_Poison
			https://minecraft.fandom.com/wiki/Slow_Falling
			https://minecraft.fandom.com/wiki/Darkness
		*/
	}

	static constexpr std::string_view roman_numbs_literal[] = { "X", "IX", "V", "IV", "I" };

	static constexpr int roman_numbs_integers[] = { 10, 9, 5, 4, 1  };

	/**
	*	@brief Converts the given integer to the roman value
	*
	*	A leading whitespace will be added to the string
	*	@param num if 0 returns empty string
	*/
	std::string_view level( int num );
}

#endif // Linux my beloved
