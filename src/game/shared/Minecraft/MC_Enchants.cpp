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

#include "MC_Enchants.h"

CEnchants::CEnchants()
{
	MCMacroDataRegister(*this, sharpness, 5);
	MCMacroDataRegister(*this, bane_of_arthropods, 5);
	MCMacroDataRegister(*this, smite, 5);
	MCMacroDataRegister(*this, fire_aspect, 2);
	MCMacroDataRegister(*this, life_steal, 5);
	MCMacroDataRegister(*this, critical_chance, 7);
	MCMacroDataRegister(*this, knockback, 2);
	MCMacroDataRegister(*this, sweeping_edge, 3);
	MCMacroDataRegister(*this, looting, 3);
}
