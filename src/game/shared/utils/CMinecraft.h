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
#include <string_view>

struct CEnchantStruct
{
    std::string_view name;
    std::string_view display_name;
    int max_level;

    constexpr CEnchantStruct(
		std::string_view name,
		std::string_view display_name,
		int max_level
	) :
		name(name),
		display_name(display_name),
		max_level(max_level)
	{}
};

class CEnchants
{
	public:
		// Sharpness adds 0.5 * level + 0.5 extra damage.
		constexpr static CEnchantStruct sharpness{ "sharpness"sv, "Sharpness"sv, 5 };

		/**
		*	@brief Each level on a sword or axe adds 2.5 ♥ × level of extra damage to each hit on arthropods only.
		*
		*	The enchantment also inflicts Slowness IV on an arthropod when hit, with a random duration between 1 and 1.5 seconds at level I increasing the maximum duration by 0.5 seconds at each level, up to 3.5 seconds with Bane of Arthropods V.
		*/
		constexpr static CEnchantStruct bane_of_arthropods{ "bane_of_arthropods"sv, "Bane of Arthropods"sv, 5 };

		/**
		*	@brief Smite increases damage dealt to un-dead mobs.
		*
		*	Each level separately adds 2.5 ♥ × level of extra damage on each hit to undead mobs.
		*/
		constexpr static CEnchantStruct smite{ "smite"sv, "Smite"sv, 5 };

		/**
		*	@brief Fire Aspect adds 4 seconds of burning per level to the target.
		*
		*	Because the first hit is caused by the item with this enchantment, the first second of fire damage is not recognized. Targets receive 3 ♥ and 6 ♥ damage with level I and II respectively.
		*/
		constexpr static CEnchantStruct fire_aspect{ "fire_aspect"sv, "Fire Ascpect"sv, 2 };

		/**
		*	@brief Life Steal restores health to the attacker in 10% per level of the dealt damage.
		*
		*	This is done before any damage multiplication and after damage deductions.
		*/
		constexpr static CEnchantStruct life_steal{ "life_steal"sv, "Life Steal"sv, 5 };

		/**
		*	@brief Critic Chance has a chance of 10% per level to multiply the damage dealth by a random value between 1.0 and 2.0.
		*
		*	This is done before any damage multiplication and after damage deductions.
		*/
		constexpr static CEnchantStruct critical_chance{ "critical_chance"sv, "Critic Chance"sv, 7 };

		/**
		*	@brief Knock Back will push the hit entity based on the received damage and enchant level.
		*/
		constexpr static CEnchantStruct knockback{ "knockback"sv, "Knock Back"sv, 2 };

		/**
		*	@brief Sweeping Edge Expanding to 512 units from the victim and hurt enemies in the path.
		*
		*   Does 25% (per level) of the original damage on each entity slashed. the formula is done after both the entity and the weapon buffs.
		* 
		*	This uses DMG_SLASH to prevent recursion, use DMG_SLASH safely.
		*/
		constexpr static CEnchantStruct sweeping_edge{ "sweeping_edge"sv, "Sweeping Edge"sv, 2 };
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

	private:
};

inline CEnchants CMinecraft::Enchants;

inline CMinecraft g_Minecraft;
