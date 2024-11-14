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

#include "CClientEnvSky.h"

LINK_ENTITY_TO_CLASS( env_sky, CClientEnvSky );

BEGIN_DATAMAP( CClientEnvSky )
    DEFINE_FIELD( m_iConstant, FIELD_INTEGER ),
    DEFINE_FIELD( m_iCurrentState, FIELD_INTEGER ),
    DEFINE_FIELD( m_flThinkDelta, FIELD_FLOAT ),
END_DATAMAP();

bool CClientEnvSky::KeyValue(KeyValueData* pkvd)
{
	if( FStrEq( pkvd->szKeyName, "m_iConstant" ) )
	{
		m_iConstant = atoi( pkvd->szValue );
	}
	else if( FStrEq( pkvd->szKeyName, "m_flThinkDelta" ) )
	{
		m_flThinkDelta = atof( pkvd->szValue );
	}
    else
    {
    	return BaseClass::KeyValue( pkvd );
    }
    return true;
}

void CClientEnvSky::Spawn()
{
	pev->effects |= EF_NODRAW;
	pev->solid = SOLID_NOT;

    if( FBitSet( pev->spawnflags, SF_SKY_ACTIVATOR_ONLY ) && m_iConstant == 1 )
    {
        CBaseEntity::Logger->warn( "{}:{} with flag {} (Activator Only) is incompatible with constant, disabling m_iConstant.", GetClassname(), entindex(), SF_SKY_ACTIVATOR_ONLY );
        m_iConstant = 0;
    }

    if( !FBitSet( pev->spawnflags, SF_SKY_START_OFF ) )
    {
        m_iCurrentState = SKY::ON;

        if( m_iConstant == 1 )
        {
            SetThink( &CClientEnvSky::SkyConstant );
            pev->nextthink = gpGlobals->time + m_flThinkDelta;
        }
    }
}

void CClientEnvSky::SkyConstant()
{
    Activate( SKY::ON );
    pev->nextthink = gpGlobals->time + m_flThinkDelta;
}

void CClientEnvSky::Activate( SKY SkyMode, CBasePlayer* pPlayer )
{
    /*
    if( pPlayer != nullptr )
    {
	    MESSAGE_BEGIN( MSG_ONE, gmsgSetSky, nullptr, pPlayer );
    }
    else
    {
        // Store current state for Toggle and late-players usage.
        m_iCurrentState = SkyMode;

	    MESSAGE_BEGIN( MSG_ALL, gmsgSetSky );
    }
    */

    MESSAGE_BEGIN(MSG_ONE, gmsgSetSky, nullptr, pPlayer);
        WRITE_BYTE( (int)m_iCurrentState ); // mode
        WRITE_COORD_VECTOR(pev->origin);    // view position
    MESSAGE_END();

    //g-cont. found all skyents
    edict_t *pent = UTIL_EntitiesInPVS( edict() );

    while( !FNullEnt( pent ) )
    {
        SetBits(pent->v.flags, FL_IMMUNE_WATER ); //hack
        pent = pent->v.chain;
    }
}

void CClientEnvSky::ActivateOne( CBasePlayer* pPlayer, USE_TYPE useType )
{
    SKY SkyMode = SKY::OFF;

    switch( useType )
    {
        case USE_ON:
            SkyMode = SKY::ON;
        break;
        case USE_OFF:
            SkyMode = SKY::OFF;
        break;
        default:
            SkyMode = ( pPlayer->m_iSkyEnvironment == SKY::OFF ? SKY::ON : SKY::OFF );
        break;
    }

    Activate( SkyMode, pPlayer );

    pPlayer->m_iSkyEnvironment = static_cast<int>( SkyMode );
}

void CClientEnvSky::Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value )
{
    if( FBitSet( pev->spawnflags, SF_SKY_ACTIVATOR_ONLY ) )
    {
        if( auto player = ToBasePlayer( pActivator ); player != nullptr )
        {
            ActivateOne( player, useType );
        }
    }
    else if( m_iConstant == 1 )
    {
        switch( useType )
        {
            case USE_ON:
            {
                if( FBitSet( pev->spawnflags, SF_SKY_START_OFF ) )
                {
                    SetThink( &CClientEnvSky::SkyConstant );
                    pev->nextthink = gpGlobals->time + m_flThinkDelta;
                    ClearBits( pev->spawnflags, SF_SKY_START_OFF );
                }
                break;
            }
            case USE_OFF:
            {
                if( !FBitSet( pev->spawnflags, SF_SKY_START_OFF ) )
                {
                    SetThink( nullptr );
                    pev->nextthink = 0;
                    SetBits( pev->spawnflags, SF_SKY_START_OFF );
                }
                break;
            }
            default:
            {
                if( !FBitSet( pev->spawnflags, SF_SKY_START_OFF ) )
                {
                    SetThink( nullptr );
                    pev->nextthink = 0;
                    SetBits( pev->spawnflags, SF_SKY_START_OFF );
                }
                else
                {
                    SetThink( &CClientEnvSky::SkyConstant );
                    pev->nextthink = gpGlobals->time + m_flThinkDelta;
                    ClearBits( pev->spawnflags, SF_SKY_START_OFF );
                }
                break;
            }
        }
    }
    else
    {
        switch( useType )
        {
            case USE_ON:
            {
                if( m_iCurrentState == SKY::OFF )
                    Activate( SKY::ON );
                break;
            }
            case USE_OFF:
            {
                if( m_iCurrentState == SKY::ON )
                    Activate( SKY::OFF );
                break;
            }
            case USE_TOGGLE:
            {
                Activate( ( m_iCurrentState == SKY::OFF ? SKY::ON : SKY::OFF ) );
                break;
            }
        }
    }
}

void SetClientEnvironmentSky( CBasePlayer* pPlayer)
{
	if( pPlayer != nullptr )
	{
        CBaseEntity* entity = nullptr;

        while( ( entity = UTIL_FindEntityByClassname( entity, "env_sky" ) ) != nullptr )
        {
            auto envsky = static_cast<CClientEnvSky*>( entity );

            if( envsky != nullptr )
            {
                if( envsky->CurrentState() == SKY::ON && envsky->m_iConstant != 1 && !FBitSet( envsky->pev->spawnflags, SF_SKY_ACTIVATOR_ONLY ) )
                {
                    envsky->Activate( SKY::ON, pPlayer );
                    break; // Found one, no sense on keep looking.
                }
            }
        }
	}
}
