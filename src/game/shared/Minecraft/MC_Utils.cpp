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

#include "MC_Utils.h"

std::string CMCUtils::level( int num )
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

HITGROUP CMCUtils::GetHitGroup(int hitgroup)
{
      switch( hitgroup )
      {
            case HITGROUP::Head:
            return HITGROUP::HELMET;
            case HITGROUP::Chest:
            return HITGROUP::CHEST;
            case HITGROUP::Legs:
            return HITGROUP::LEGS;
            case HITGROUP::Feet:
            return HITGROUP::BOOTS;
      }
      return HITGROUP::NONE;
}
