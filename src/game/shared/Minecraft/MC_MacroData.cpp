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

#include "MC_MacroData.h"

MCMacroData::MCMacroData()
{
}

std::string MCMacroData::level( int num )
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

// We're going to use this or titles?
std::string MCMacroData::format_level( const char* str1, std::optional<int> num )
{
	if( num != std::nullopt )
		return fmt::format( "{} {}", str1, level( num.value() ) );
	return std::string( str1 );
}

bool MCMacroData::Exists(std::string name)
{
	return data.find( name ) != data.end();
}

int MCMacroData::GetLevel(std::string name, std::optional<int> num)
{
	if( !Exists(name) || num == std::nullopt )
		return 1;

	return std::clamp( num.value(), 0, data.value(name,1) );
}
