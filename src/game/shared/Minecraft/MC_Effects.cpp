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

#include "MC_Effects.h"

CEffects::CEffects()
{
	MCMacroDataRegister(*this, fire, 2);
	MCMacroDataRegister(*this, speed, 2);
	MCMacroDataRegister(*this, slowness, 4);
	MCMacroDataRegister(*this, fire_resistance, 0);
	MCMacroDataRegister(*this, haste, 0);
	MCMacroDataRegister(*this, fatigue, 0);
	MCMacroDataRegister(*this, strength, 0);
	MCMacroDataRegister(*this, instant_health, 0);
	MCMacroDataRegister(*this, instant_damage, 0);
	MCMacroDataRegister(*this, jump_boost, 0);
	MCMacroDataRegister(*this, nausea, 0);
	MCMacroDataRegister(*this, regeneration, 0);
	MCMacroDataRegister(*this, resistance, 0);
	MCMacroDataRegister(*this, water_breathing, 0);
	MCMacroDataRegister(*this, invisibility, 0);
	MCMacroDataRegister(*this, blindness, 0);
	MCMacroDataRegister(*this, night_vision, 0);
	MCMacroDataRegister(*this, weakness, 0);
	MCMacroDataRegister(*this, poison, 0);
	MCMacroDataRegister(*this, wither, 0);
	MCMacroDataRegister(*this, absorption, 0);
	MCMacroDataRegister(*this, glowing, 0);
	MCMacroDataRegister(*this, levitation, 0);
	MCMacroDataRegister(*this, luck, 0);
	MCMacroDataRegister(*this, bad_luck, 0);
	MCMacroDataRegister(*this, fatal_poison, 0);
	MCMacroDataRegister(*this, slow_falling, 0);
	MCMacroDataRegister(*this, darkness, 0);
}
