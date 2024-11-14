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

#include "cbase.h"
#include "UserMessages.h"

#pragma once

#define SF_SKY_START_OFF ( 1 << 0 )
#define SF_SKY_ACTIVATOR_ONLY ( 1 << 1 )

enum SKY : int
{
    OFF = 0,
    ON
};

//=========================================================
// LRC - env_sky, an unreal tournament-style sky effect
//=========================================================
class CClientEnvSky : public CBaseEntity
{
	DECLARE_CLASS( CClientEnvSky, CBaseEntity );
	DECLARE_DATAMAP();

    public:
        int m_iConstant;
        float m_flThinkDelta = 0.5;
        void SkyConstant();
        SKY CurrentState() { return m_iCurrentState; }
        void Spawn() override;
        void Activate( SKY SkyMode, CBasePlayer* pPlayer = nullptr );
        bool KeyValue( KeyValueData* pkvd ) override;
        // Activate for the given target only, NOTE that this stores the state in CBasePlayer::m_iSkyEnvironment instead of self m_iCurrentState
        void ActivateOne( CBasePlayer* pPlayer, USE_TYPE useType = USE_SET ); // Should be USE_UNSET -TODO
        void Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value ) override;

    private:
        SKY m_iCurrentState = SKY::OFF;
};

void SetClientEnvironmentSky( CBasePlayer* pPlayer);
