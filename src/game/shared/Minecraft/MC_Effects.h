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

#include "MC_MacroData.h"

class CEffects : public MCMacroData
{
	public:

		std::string fire;
		std::string speed;
		std::string slowness;
		std::string fire_resistance;
		std::string haste;
		std::string fatigue;
		std::string strength;
		std::string instant_health;
		std::string instant_damage;
		std::string jump_boost;
		std::string nausea;
		std::string regeneration;
		std::string resistance;
		std::string water_breathing;
		std::string invisibility;
		std::string blindness;
		std::string night_vision;
		std::string weakness;
		std::string poison;
		std::string wither;
		std::string absorption;
		std::string glowing;
		std::string levitation;
		std::string luck;
		std::string bad_luck;
		std::string fatal_poison;
		std::string slow_falling;
		std::string darkness;

		CEffects();
};
