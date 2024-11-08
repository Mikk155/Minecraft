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

#pragma once

#include <unordered_map>
#include <vector>
#include <string_view>

#include "entities/cbase.h"

#define Unbreakable -100
#define FUnbreakable( var ) ( var == Unbreakable )

struct CInventory
{
	CBaseEntity* pItem;
	int max_stack;
	int amount;

	CInventory( CBaseEntity* pItem = nullptr, int max_stack = 64, int amount = 1 )
		: pItem(pItem), max_stack(max_stack), amount(amount) {}
};

enum class InventorySlot : int
{
	LeftHand = 0,
	Hotbar1, Hotbar2, Hotbar3, Hotbar4, Hotbar5, Hotbar6, Hotbar7, Hotbar8, Hotbar9,

	ArmorHelmet,
	ArmorChest,
	ArmorLegs,
	ArmorBoots,

	// These two may be renamed or removed -Mikk
	FirstItemSlot, /* ... ... */ LastItemSlot = 41,

	// Arrows slots, only projectiles for bow and crossbow goes here.
	Arrows = 42,
};

enum class DMG : int
{
	GENERIC	= 0,
	CRUSH	= (1 << 0 ),	// crushed by falling or moving object
	SLASH	= (1 << 2 ),	// cut, clawed, stabbed
	FIRE	= (1 << 3 ),	// heat burned
	FREEZE	= (1 << 4 ),	// frozen
	FALL	= (1 << 5 ),	// fell too far
	BLAST	= (1 << 6 ),	// explosive blast damage
	DROWN	= (1 << 14),	// Drowning
	POISON	= (1 << 17)		// blood poisioning
};

struct CLevelStruct
{
    std::string_view name;
    std::string_view display_name;
    int max_level;

    constexpr CLevelStruct(
		std::string_view name,
		std::string_view display_name,
		int max_level
	) :
		name(name),
		display_name(display_name),
		max_level(max_level)
	{}
};

class CEffects
{
	public:

		/**
		*	@brief Fire burns the victim and apply 0.5 ♥ every second for 4 seconds,
        *
        *   When level 2 the seconds expands to 8.
		* 
		*	When level 3 it apply every half of a second
		*/
		constexpr static CLevelStruct fire{ "fire"sv, "Fire"sv, 3 };

		/**
		*	@brief Speed is a status effect that increases an entity's walking speed by 20% multiplied by the effect level. It expands a player's field of view (FOV)
		*/
		constexpr static CLevelStruct speed{ "speed"sv, "Speed"sv, 2 };

		/**
		*	@brief Slowness decreases walking speed by 15% × level and contracts the player's field of view accordingly.
		*/
		constexpr static CLevelStruct slowness{ "slowness"sv, "Slowness"sv, 4 };

		/**
		*	@brief Grants full inmunity to fire
		*/
		constexpr static CLevelStruct fire_resistance{ "fire_resistance"sv, "Fire Resistance"sv, 0 };

		// https://minecraft.fandom.com/wiki/Haste
		constexpr static CLevelStruct haste{ "haste"sv, "Haste"sv, 0 };

		// https://minecraft.fandom.com/wiki/Mining_Fatigue
		constexpr static CLevelStruct fatigue{ "fatigue"sv, "Fatigue"sv, 0 };

		// https://minecraft.fandom.com/wiki/Strength
		constexpr static CLevelStruct strength{ "strength"sv, "Strength"sv, 0 };

		// https://minecraft.fandom.com/wiki/Instant_Health
		constexpr static CLevelStruct instant_health{ "instant_health"sv, "Instant Health"sv, 0 };

		// https://minecraft.fandom.com/wiki/Instant_Damage
		constexpr static CLevelStruct instant_damage{ "instant_damage"sv, "Instant Damage"sv, 0 };

		// https://minecraft.fandom.com/wiki/Jump_Boost
		constexpr static CLevelStruct jump_boost{ "jump_boost"sv, "Jump Boost"sv, 0 };

		// https://minecraft.fandom.com/wiki/Nausea
		constexpr static CLevelStruct nausea{ "nausea"sv, "Nausea"sv, 0 };

		// https://minecraft.fandom.com/wiki/Regeneration
		constexpr static CLevelStruct regeneration{ "regeneration"sv, "Regeneration"sv, 0 };

		// https://minecraft.fandom.com/wiki/Resistance
		constexpr static CLevelStruct resistance{ "resistance"sv, "Resistance"sv, 0 };

		// https://minecraft.fandom.com/wiki/Water_Breathing
		constexpr static CLevelStruct water_breathing{ "water_breathing"sv, "Water Breathing"sv, 0 };

		// https://minecraft.fandom.com/wiki/Invisibility
		constexpr static CLevelStruct invisibility{ "invisibility"sv, "Invisibility"sv, 0 };

		// https://minecraft.fandom.com/wiki/Blindness
		constexpr static CLevelStruct blindness{ "blindness"sv, "Blindness"sv, 0 };

		// https://minecraft.fandom.com/wiki/Night_Vision
		constexpr static CLevelStruct night_vision{ "night_vision"sv, "Night Vision"sv, 0 };

		// https://minecraft.fandom.com/wiki/Weakness
		constexpr static CLevelStruct weakness{ "weakness"sv, "Weakness"sv, 0 };

		// https://minecraft.fandom.com/wiki/Poison
		constexpr static CLevelStruct poison{ "poison"sv, "Poison"sv, 0 };

		// https://minecraft.fandom.com/wiki/Wither_(effect)
		constexpr static CLevelStruct wither{ "wither"sv, "Wither"sv, 0 };

		// https://minecraft.fandom.com/wiki/Absorption
		constexpr static CLevelStruct absorption{ "absorption"sv, "Absorption"sv, 0 };

		// https://minecraft.fandom.com/wiki/Glowing
		constexpr static CLevelStruct glowing{ "glowing"sv, "Glowing"sv, 0 };

		// https://minecraft.fandom.com/wiki/Levitation
		constexpr static CLevelStruct levitation{ "levitation"sv, "Levitation"sv, 0 };

		// https://minecraft.fandom.com/wiki/Luck
		constexpr static CLevelStruct luck{ "luck"sv, "Luck"sv, 0 };

		// https://minecraft.fandom.com/wiki/Bad_Luck
		constexpr static CLevelStruct bad_luck{ "bad_luck"sv, "Bad_Luck"sv, 0 };

		// https://minecraft.fandom.com/wiki/Fatal_Poison
		constexpr static CLevelStruct fatal_poison{ "fatal_poison"sv, "Fatal Poison"sv, 0 };

		// https://minecraft.fandom.com/wiki/Slow_Falling
		constexpr static CLevelStruct slow_falling{ "slow_falling"sv, "Slow Falling"sv, 0 };

		// https://minecraft.fandom.com/wiki/Darkness
		constexpr static CLevelStruct darkness{ "darkness"sv, "Darkness"sv, 0 };
};

class CEnchants
{
	public:
		// Sharpness adds 0.5 * level + 0.5 extra damage.
		constexpr static CLevelStruct sharpness{ "sharpness"sv, "Sharpness"sv, 5 };

		/**
		*	@brief Each level on a sword or axe adds 2.5 ♥ × level of extra damage to each hit on arthropods only.
		*
		*	The enchantment also inflicts Slowness IV on an arthropod when hit, with a random duration between 1 and 1.5 seconds at level I increasing the maximum duration by 0.5 seconds at each level, up to 3.5 seconds with Bane of Arthropods V.
		*/
		constexpr static CLevelStruct bane_of_arthropods{ "bane_of_arthropods"sv, "Bane of Arthropods"sv, 5 };

		/**
		*	@brief Smite increases damage dealt to un-dead mobs.
		*
		*	Each level separately adds 2.5 ♥ × level of extra damage on each hit to undead mobs.
		*/
		constexpr static CLevelStruct smite{ "smite"sv, "Smite"sv, 5 };

		/**
		*	@brief Fire Aspect adds 4 seconds of burning per level to the target.
		*
		*	Because the first hit is caused by the item with this enchantment, the first second of fire damage is not recognized. Targets receive 3 ♥ and 6 ♥ damage with level I and II respectively.
		*/
		constexpr static CLevelStruct fire_aspect{ "fire_aspect"sv, "Fire Ascpect"sv, 2 };

		/**
		*	@brief Life Steal restores health to the attacker in 10% per level of the dealt damage.
		*
		*	This is done before any damage multiplication and after damage deductions.
		*/
		constexpr static CLevelStruct life_steal{ "life_steal"sv, "Life Steal"sv, 5 };

		/**
		*	@brief Critic Chance has a chance of 10% per level to multiply the damage dealth by a random value between 1.0 and 2.0.
		*
		*	This is done before any damage multiplication and after damage deductions.
		*/
		constexpr static CLevelStruct critical_chance{ "critical_chance"sv, "Critic Chance"sv, 7 };

		/**
		*	@brief Knock Back will push the hit entity based on the received damage and enchant level.
		*/
		constexpr static CLevelStruct knockback{ "knockback"sv, "Knock Back"sv, 2 };

		/**
		*	@brief Sweeping Edge Expanding to 512 units from the victim and hurt enemies in the path.
		*
		*   Does 25% (per level) of the original damage on each entity slashed. the formula is done after both the entity and the weapon buffs.
		* 
		*	This uses DMG_SLASH to prevent recursion, use DMG_SLASH safely.
		*/
		constexpr static CLevelStruct sweeping_edge{ "sweeping_edge"sv, "Sweeping Edge"sv, 3 };
};

/**
 *	@brief Minecraft utilities
 */
class CMinecraft final
{
	public:

		/**
		*	@brief Converts the given integer to the roman value
		*	@param num if greater than 10, it will return a simple conversion from the string.
		*/
		std::string_view level(int num);
		// Returns a formatted string using level(int num)
		std::string format_level(const char* str1, int num = 0);

		static CEnchants Enchants;

		static CEffects Effects;

	private:
};

inline CEnchants CMinecraft::Enchants;

inline CEffects CMinecraft::Effects;

inline CMinecraft g_Minecraft;
