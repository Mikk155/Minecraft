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

std::string_view CMinecraft::level( int num )
{
	switch( num )
	{
		case 1: return "I"sv;
		case 2: return "II"sv;
		case 3: return "III"sv;
		case 4: return "IV"sv;
		case 5: return "V"sv;
		case 6: return "VI"sv;
		case 7: return "VII"sv;
		case 8: return "VIII"sv;
		case 9: return "IX"sv;
		case 10: return "X"sv;
	}

	return std::to_string( num );
}

std::string CMinecraft::format_level( const char* str1, int num )
{
	if( num != 0 )
		return fmt::format( "{} {}", str1, level( num ) );
	return std::string( str1 );
}


bool CEffects::Exists(std::string_view effect_name)
{
    return std::find( _effects_.begin(), _effects_.end(), effect_name ) != _effects_.end();
}
