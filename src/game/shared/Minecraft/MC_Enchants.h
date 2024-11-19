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

class CEnchants
{
	public:

		std::string sharpness;
		std::string bane_of_arthropods;
		std::string smite;
		std::string fire_aspect;
		std::string life_steal;
		std::string critical_chance;
		std::string knockback;
		std::string sweeping_edge;
		std::string looting;

		CEnchants();
};
