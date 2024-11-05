/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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

#include "hud.h"

bool CHudSetSky::Init()
{
	gHUD.AddHudElem(this);

	g_ClientUserMessages.RegisterHandler("SetSky", &CHudSetSky::MsgFunc_SetSky, this);
	g_ClientUserMessages.RegisterHandler("CamData", &CHudSetSky::MsgFunc_CamData, this);

	m_iFlags = HUD_ACTIVE;
	m_iSkyMode = 0;
	viewEntityIndex = 0;
	viewFlags = 0;

	return true;
}

void CHudSetSky::MsgFunc_SetSky(const char* pszName, BufferReader& reader)
{
	m_iSkyMode = reader.ReadByte();
	m_vecSkyPos.x = reader.ReadCoord();
	m_vecSkyPos.y = reader.ReadCoord();
	m_vecSkyPos.z = reader.ReadCoord();
}

void CHudSetSky::MsgFunc_CamData(const char* pszName, BufferReader& reader)
{
	viewEntityIndex = reader.ReadShort();
	viewFlags = reader.ReadShort();
}