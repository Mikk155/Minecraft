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

// Inventory networking enum
enum class InventoryNetwork : int
{
	// Close the inventory
	Close = 0,
	// Receive a item
	Item,
	// Receive a data for an item
	Data,
	// Open the inventory
	Open
};

enum class InventorySlot : int
{
	LeftHand = 0,
	Hotbar1, Hotbar2, Hotbar3, Hotbar4, Hotbar5, Hotbar6, Hotbar7, Hotbar8, Hotbar9,

	ArmorHelmet,
	ArmorChest,
	ArmorLegs,
	ArmorBoots,
	Arrows,
	MONSTER_MAX_SLOTS = 16,

	// Inventory slots for player
	FirstItemSlot = 16, /* ... ... */ LastItemSlot = 42,

	InvalidAddSlot = 13,
	MAX_SLOTS = 43
};

enum class DMG : int
{
	// Generic damage, has no special effects
	GENERIC	= 0,
	CRUSH	= (1 << 0 ),	// crushed by falling or moving object
	// This effect prevents recursion when using Sweeping Edge
	SLASH	= (1 << 1 ),
	// Displays fire on the HUD
	FIRE	= (1 << 2 ),
	// Apply effect fire I to the victim
	LAVA	= (1 << 3 ),
	FREEZE	= (1 << 4 ),	// frozen
	FALL	= (1 << 5 ),	// fell too far
	// explosive blast damage
	BLAST	= (1 << 6 ),
	DROWN	= (1 << 14),	// Drowning
	POISON	= (1 << 17)		// blood poisioning
};

struct CLevelStruct
{
    std::string_view name;
    std::string_view display_name;
    int max_level;

    constexpr CLevelStruct(std::string_view name, std::string_view display_name, int max_level)
		: name(name), display_name(display_name), max_level(max_level) { }
};

struct CEffectsData
{
	// Effect name
	std::string_view name;
	// Effect level
	int level;
	// Time till the effect ends
	float end;
	// Time, used for comparitions in Think, is the cooldown till apply the effect
	float time;
	// attacker are for wither, poison, fire etc
	CBaseEntity* attacker;
	// Last time, used internaly for comparing with float time
	float last_time;

	CEffectsData(std::string_view name, float level, float end, float time = 0.0f, CBaseEntity* attacker = nullptr) : name(name), level(level), end(end), time(time), attacker(attacker) {}
};

class CEffects
{
	public:
		bool Exists(std::string_view effect_name);
		constexpr static CLevelStruct fire{ "fire"sv, "Fire"sv, 2 };
		constexpr static CLevelStruct speed{ "speed"sv, "Speed"sv, 2 };
		constexpr static CLevelStruct slowness{ "slowness"sv, "Slowness"sv, 4 };
		constexpr static CLevelStruct fire_resistance{ "fire_resistance"sv, "Fire Resistance"sv, 0 };
		constexpr static CLevelStruct haste{ "haste"sv, "Haste"sv, 0 };
		constexpr static CLevelStruct fatigue{ "fatigue"sv, "Fatigue"sv, 0 };
		constexpr static CLevelStruct strength{ "strength"sv, "Strength"sv, 0 };
		constexpr static CLevelStruct instant_health{ "instant_health"sv, "Instant Health"sv, 0 };
		constexpr static CLevelStruct instant_damage{ "instant_damage"sv, "Instant Damage"sv, 0 };
		constexpr static CLevelStruct jump_boost{ "jump_boost"sv, "Jump Boost"sv, 0 };
		constexpr static CLevelStruct nausea{ "nausea"sv, "Nausea"sv, 0 };
		constexpr static CLevelStruct regeneration{ "regeneration"sv, "Regeneration"sv, 0 };
		constexpr static CLevelStruct resistance{ "resistance"sv, "Resistance"sv, 0 };
		constexpr static CLevelStruct water_breathing{ "water_breathing"sv, "Water Breathing"sv, 0 };
		constexpr static CLevelStruct invisibility{ "invisibility"sv, "Invisibility"sv, 0 };
		constexpr static CLevelStruct blindness{ "blindness"sv, "Blindness"sv, 0 };
		constexpr static CLevelStruct night_vision{ "night_vision"sv, "Night Vision"sv, 0 };
		constexpr static CLevelStruct weakness{ "weakness"sv, "Weakness"sv, 0 };
		constexpr static CLevelStruct poison{ "poison"sv, "Poison"sv, 0 };
		constexpr static CLevelStruct wither{ "wither"sv, "Wither"sv, 0 };
		constexpr static CLevelStruct absorption{ "absorption"sv, "Absorption"sv, 0 };
		constexpr static CLevelStruct glowing{ "glowing"sv, "Glowing"sv, 0 };
		constexpr static CLevelStruct levitation{ "levitation"sv, "Levitation"sv, 0 };
		constexpr static CLevelStruct luck{ "luck"sv, "Luck"sv, 0 };
		constexpr static CLevelStruct bad_luck{ "bad_luck"sv, "Bad_Luck"sv, 0 };
		constexpr static CLevelStruct fatal_poison{ "fatal_poison"sv, "Fatal Poison"sv, 0 };
		constexpr static CLevelStruct slow_falling{ "slow_falling"sv, "Slow Falling"sv, 0 };
		constexpr static CLevelStruct darkness{ "darkness"sv, "Darkness"sv, 0 };
	
	private:

		// -MC How to propertly set this automatically through CLevelStruct constructor?
		std::vector< std::string_view > _effects_ = {
			fire.name, speed.name, slowness.name, fire_resistance.name, haste.name, fatigue.name,
			strength.name, instant_health.name, instant_damage.name, jump_boost.name,
			nausea.name, regeneration.name, resistance.name, water_breathing.name,
			invisibility.name, blindness.name, night_vision.name, weakness.name,
			poison.name, wither.name, absorption.name, glowing.name,
			levitation.name, luck.name, bad_luck.name,
			fatal_poison.name, slow_falling.name,
			darkness.name
		};
};

class CEnchants
{
	public:
		constexpr static CLevelStruct sharpness{ "sharpness"sv, "Sharpness"sv, 5 };
		constexpr static CLevelStruct bane_of_arthropods{ "bane_of_arthropods"sv, "Bane of Arthropods"sv, 5 };
		constexpr static CLevelStruct smite{ "smite"sv, "Smite"sv, 5 };
		constexpr static CLevelStruct fire_aspect{ "fire_aspect"sv, "Fire Ascpect"sv, 2 };
		constexpr static CLevelStruct life_steal{ "life_steal"sv, "Life Steal"sv, 5 };
		constexpr static CLevelStruct critical_chance{ "critical_chance"sv, "Critic Chance"sv, 7 };
		constexpr static CLevelStruct knockback{ "knockback"sv, "Knock Back"sv, 2 };
		constexpr static CLevelStruct sweeping_edge{ "sweeping_edge"sv, "Sweeping Edge"sv, 3 };
		constexpr static CLevelStruct looting{ "looting"sv, "Looting"sv, 3 };
};

/**
 *	@brief Minecraft utilities
 */
class CMinecraft final
{
	public:

		/**
		*	@brief Converts the given integer to the roman value
		*	@param num if greater than 10, it will return a simple conversion to string.
		*/
		std::string level(int num);
		// Returns a formatted string using level(int num)
		std::string format_level(const char* str1, int num = 0);

		static CEnchants Enchants;

		static CEffects Effects;
};

inline CEnchants CMinecraft::Enchants;

inline CEffects CMinecraft::Effects;

inline CMinecraft g_Minecraft;
