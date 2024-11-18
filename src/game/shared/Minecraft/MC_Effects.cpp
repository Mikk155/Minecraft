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

void CEffects::RemoveAll(CBaseMonster* entity)
{
	for( auto& [key, effect] : entity->effects)
	{
		delete effect;
	}
	entity->effects.clear();
}

void CEffects::Remove(CBaseMonster* entity, std::string_view effect_name)
{
	auto it = entity->effects.find( effect_name );

	if( it != entity->effects.end() )
	{
		delete it->second;
		entity->effects.erase(it);
	}
}

void CEffects::Add(CBaseMonster* entity, std::string_view effect_name, const EffectInfo&& effect)
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

	effect.duration == gpGlobals->time + effect.duration;

	if( auto it = entity->effects.find( effect_name ); it != entity->effects.end() )
	{
		if( it->second->duration < effect.duration )
		{
			it->second->duration = effect.duration;
			it->second->attacker = effect.attacker;
			return;
		}
		delete it->second;
		entity->effects.erase(it);
	}

	entity->effects[ effect_name ] = new EffectInfo( std::move(effect) );
}

CEffects::CEffects()
{
}
