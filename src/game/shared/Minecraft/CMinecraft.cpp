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

#include "CMinecraft.h"

CEffects::CEffects()
{
	data.emplace( "fire", 2 );
	data.emplace( "speed", 2 );
	data.emplace( "slowness", 4 );
	data.emplace( "fire_resistance", 0 );
	data.emplace( "haste", 0 );
	data.emplace( "fatigue", 0 );
	data.emplace( "strength", 0 );
	data.emplace( "instant_health", 0 );
	data.emplace( "instant_damage", 0 );
	data.emplace( "jump_boost", 0 );
	data.emplace( "nausea", 0 );
	data.emplace( "regeneration", 0 );
	data.emplace( "resistance", 0 );
	data.emplace( "water_breathing", 0 );
	data.emplace( "invisibility", 0 );
	data.emplace( "blindness", 0 );
	data.emplace( "night_vision", 0 );
	data.emplace( "weakness", 0 );
	data.emplace( "poison", 0 );
	data.emplace( "wither", 0 );
	data.emplace( "absorption", 0 );
	data.emplace( "glowing", 0 );
	data.emplace( "levitation", 0 );
	data.emplace( "luck", 0 );
	data.emplace( "bad_luck", 0 );
	data.emplace( "fatal_poison", 0 );
	data.emplace( "slow_falling", 0 );
	data.emplace( "darkness", 0 );
}

CEnchants::CEnchants()
{
	data.emplace( "sharpness", 5 );
	data.emplace( "bane_of_arthropods", 5 );
	data.emplace( "smite", 5 );
	data.emplace( "fire_aspect", 2 );
	data.emplace( "life_steal", 5 );
	data.emplace( "critical_chance", 7 );
	data.emplace( "knockback", 2 );
	data.emplace( "sweeping_edge", 3 );
	data.emplace( "looting", 3 );
}

std::string CMinecraft::level( int num )
{
	switch( num )
	{
		case 1: return "I";
		case 2: return "II";
		case 3: return "III";
		case 4: return "IV";
		case 5: return "V";
		case 6: return "VI";
		case 7: return "VII";
		case 8: return "VIII";
		case 9: return "IX";
		case 10: return "X";
	}

	return std::to_string( num );
}

std::string CMinecraft::format_level( const char* str1, int num )
{
	if( num != 0 )
		return fmt::format( "{} {}", str1, level( num ) );
	return std::string( str1 );
}
