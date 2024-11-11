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

class CBlockLava : public CBaseTrigger
{
	DECLARE_CLASS( CBlockLava, CBaseTrigger );

public:
	void Spawn() override;
	void HurtTouch(CBaseEntity* pOther);
	void RadiationThink();
private:
    m_flNextThink;
};

LINK_ENTITY_TO_CLASS( block_lava, CBlockLava );

void CBlockLava::Spawn()
{
	InitTrigger();
	SetTouch(&CBlockLava::HurtTouch);
    SetThink(&CBlockLava::RadiationThink);
    pev->nextthink = gpGlobals->time + RANDOM_FLOAT(0.0, 0.5);

	SetOrigin(pev->origin); // Link into the list
}

void CBlockLava::HurtTouch(CBaseEntity* pOther)
{
    if( m_flNextThink < gpGlobals->time )
    {
        CBaseEntity* pList[256];

        int count = UTIL_EntitiesInBox( pList, 256, pev->absmin, pev->absmax, ( FL_CLIENT | FL_MONSTER ) );

        for( int i = 0; i < count; i++ )
        {
			//pList[i]->TraceAttack();
        }
    }
}

void CTriggerHurt::RadiationThink()
{
	float flRange;
	Vector vecSpot1;
	Vector vecSpot2;
	Vector vecRange;
	Vector origin;
	Vector view_ofs;

	// check to see if a player is in pvs
	// if not, continue

	// set origin to center of trigger so that this check works
	origin = pev->origin;
	view_ofs = pev->view_ofs;

	pev->origin = (pev->absmin + pev->absmax) * 0.5;
	pev->view_ofs = pev->view_ofs * 0.0;

	CBasePlayer* player = UTIL_FindClientInPVS(this);

	pev->origin = origin;
	pev->view_ofs = view_ofs;

	// reset origin

	if (player)
	{
		// get range to player;

		vecSpot1 = (pev->absmin + pev->absmax) * 0.5;
		vecSpot2 = (player->pev->absmin + player->pev->absmax) * 0.5;

		vecRange = vecSpot1 - vecSpot2;
		flRange = vecRange.Length();

		// if player's current geiger counter range is larger
		// than range to this trigger hurt, reset player's
		// geiger counter range

		if (player->m_flgeigerRange >= flRange)
			player->m_flgeigerRange = flRange;
	}

	pev->nextthink = m_flNextThink = gpGlobals->time + 0.25;
}
