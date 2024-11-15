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

#include "MC_ConstDefs.h"

#include <unordered_map>
#include <vector>
#include <string_view>

#include "json_fwd.h"

#include "entities/cbase.h"

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
		CEffects();
		json data = json::object();
};

class CEnchants
{
	public:
		CEnchants();
		json data = json::object();
};

/**
 *	@brief Minecraft utilities
 */
class CMinecraft final
{
	//CMinecraft();

	public:

		/**
		*	@brief Converts the given integer to the roman value
		*	@param num if greater than 10, it will return a simple conversion to string.
		*/
		std::string level(int num);
		// Returns a formatted string using level(int num)
		std::string format_level(const char* str1, int num = 0);

		static CEffects Effects;
		static CEnchants Enchants;
};

inline CEffects CMinecraft::Effects;
inline CEnchants CMinecraft::Enchants;

inline CMinecraft g_Minecraft;
