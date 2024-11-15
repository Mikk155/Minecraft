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

#include "entities/cbase.h"

#define MCMacroDataRegister( instance, effect_id, max_value ) \
    (instance).##effect_id## = #effect_id; \
		(instance).data.emplace( #effect_id, max_value);

class MCMacroData
{
	public:

		MCMacroData();

		json data = json::object();

		/**
		*	@brief Converts the given integer to the roman value
		*	@param num if greater than 10, it will return a simple conversion to string.
		*/
		std::string level(int num);
		/**
		*	@brief Formats the given string to use the given roman value
		*	@param num if greater than 10, it will return a simple conversion to string. if 0 it will return str1 as is
		*/
		std::string format_level(const char* str1, std::optional<int> num = std::nullopt);

		/**
		*	@brief Returns whatever ``name`` exists in this class
		*/
		bool Exists(std::string name);

		/**
		*	@brief Returns a clamped value for the given level.
		*
		*	Returns 1 if ``name`` doesn't exists or not ``num`` given.
		*/
		int GetLevel( std::string name, std::optional<int> num = std::nullopt );
};
