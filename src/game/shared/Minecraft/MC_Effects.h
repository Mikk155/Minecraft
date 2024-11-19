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

#include "CMinecraft.h"

struct EffectInfo
{
	std::string_view effect_id;
	float duration = 0;
	int level = 0;
	CBaseEntity* attacker = nullptr;
	float cooldown = 0;
	float time = 0;

	EffectInfo(
		std::string_view effect_id,
		float duration,
		int level = 0,
		CBaseEntity* attacker = nullptr,
		float cooldown = 0
	) :
	effect_id(effect_id),
	duration(duration),
	level(level),
	attacker(attacker),
	cooldown(cooldown)
	{}
};

class CEffects
{
	private:
		std::string_view stv(std::string_view s, int i) { return ( i > 0 ? fmt::format( "{}.{}"sv, s, i ) : s ); }

	public:

		void RemoveAll(CBaseEntity* entity);
		void Remove(CBaseEntity* entity, std::string_view effect_name);
		void Add(CBaseEntity* entity, std::string_view effect_name, const EffectInfo&& effect);

		std::string_view fire(int i){return stv("fire"sv,i);}
		std::string_view speed(int i){return stv("speed"sv,i);}
		std::string_view slowness(int i){return stv("slowness"sv,i);}
		std::string_view fire_resistance(int i){return stv("fire_resistance"sv,i);}
		std::string_view haste(int i){return stv("haste"sv,i);}
		std::string_view fatigue(int i){return stv("fatigue"sv,i);}
		std::string_view strength(int i){return stv("strength"sv,i);}
		std::string_view instant_health(int i){return stv("instant_health"sv,i);}
		std::string_view instant_damage(int i){return stv("instant_damage"sv,i);}
		std::string_view jump_boost(int i){return stv("jump_boost"sv,i);}
		std::string_view nausea(int i){return stv("nausea"sv,i);}
		std::string_view regeneration(int i){return stv("regeneration"sv,i);}
		std::string_view resistance(int i){return stv("resistance"sv,i);}
		std::string_view water_breathing(int i){return stv("water_breathing"sv,i);}
		std::string_view invisibility(int i){return stv("invisibility"sv,i);}
		std::string_view blindness(int i){return stv("blindness"sv,i);}
		std::string_view night_vision(int i){return stv("night_vision"sv,i);}
		std::string_view weakness(int i){return stv("weakness"sv,i);}
		std::string_view poison(int i){return stv("poison"sv,i);}
		std::string_view wither(int i){return stv("wither"sv,i);}
		std::string_view absorption(int i){return stv("absorption"sv,i);}
		std::string_view glowing(int i){return stv("glowing"sv,i);}
		std::string_view levitation(int i){return stv("levitation"sv,i);}
		std::string_view luck(int i){return stv("luck"sv,i);}
		std::string_view bad_luck(int i){return stv("bad_luck"sv,i);}
		std::string_view fatal_poison(int i){return stv("fatal_poison"sv,i);}
		std::string_view slow_falling(int i){return stv("slow_falling"sv,i);}
		std::string_view darkness(int i){return stv("darkness"sv,i);}

		CEffects();
};
