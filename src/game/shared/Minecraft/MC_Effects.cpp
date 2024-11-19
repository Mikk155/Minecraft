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

void CEffects::RemoveAll(CBaseEntity* entity)
{
	if( auto monster = ToBaseMonster(entity); monster != nullptr )
	{
		for( auto& [key, effect] : monster->effects)
		{
			delete effect;
		}
		monster->effects.clear();
	}
}

void CEffects::Remove(CBaseEntity* entity, std::string_view effect_name)
{
	if( auto monster = ToBaseMonster(entity); monster != nullptr )
	{
		auto it = monster->effects.find( effect_name );

		if( it != monster->effects.end() )
		{
			delete it->second;
			monster->effects.erase(it);
		}
	}
}

void CEffects::Add(CBaseEntity* entity, std::string_view effect_name, const EffectInfo&& effect)
{
	if( auto monster = ToBaseMonster(entity); monster != nullptr )
	{
		if( auto player = ToBasePlayer(entity); player != nullptr )
		{
/*
			MESSAGE_BEGIN( MSG_BROADCAST, gmsgEffect, nullptr, player );
				WRITE_STRING( std::string( effect.effect_id ).c_str() );
				WRITE_BYTE( effect.level );
				WRITE_COORD( effect.duration );
			MESSAGE_END();
*/
		}

		effect.duration = gpGlobals->time + effect.duration;

		if( auto it = monster->effects.find( effect_name ); it != monster->effects.end() )
		{
			if( it->second->duration < effect.duration )
			{
				it->second->duration = effect.duration;
				it->second->attacker = effect.attacker;
				return;
			}
			delete it->second;
			monster->effects.erase(it);
		}

		monster->effects[ effect_name ] = new EffectInfo( std::move(effect) );
	}
}

CEffects::CEffects()
{
}
