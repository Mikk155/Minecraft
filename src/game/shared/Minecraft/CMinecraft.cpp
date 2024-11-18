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

HITGROUP CMinecraft::GetHitGroup(int hitgroup)
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
