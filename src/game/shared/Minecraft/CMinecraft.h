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

/**
 *	@file Minecraft
 */

#pragma once

#include "../../server/entities/cbase.h"

#include "MC_ConstDefs.h"

#include "MC_Effects.h"
#include "MC_Enchants.h"
#include "MC_Utils.h"

/**
 *	@brief Minecraft
 */
class CMinecraft final
{
	public:
		static CEffects Effects;
		static CEnchants Enchants;
		static CMCUtils Utils;
};

inline CEffects CMinecraft::Effects;
inline CEnchants CMinecraft::Enchants;
inline CMCUtils CMinecraft::Utils;

inline CMinecraft g_Minecraft;
