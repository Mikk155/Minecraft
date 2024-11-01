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
//
// inventory.cpp
//

#include <string>

#include "hud.h"

#include "vgui_TeamFortressViewport.h"

bool CHudInventory::Init()
{
	g_ClientUserMessages.RegisterHandler("Inventory", &CHudInventory::MsgFunc_Inventory, this);

	m_iFlags |= HUD_ACTIVE;
	m_fOn = false;

	gHUD.AddHudElem(this);
	
	return true;
}

void CHudInventory::Reset()
{
	m_fOn = false;
}

bool CHudInventory::VidInit()
{
	m_hudSprite = SPR_Load("sprites/gui/inventory.spr");

	return true;
}

bool CHudInventory::Draw(float flTime)
{
	// don't draw the menu if the scoreboard is being shown
	if (gHUD.m_Scoreboard.m_iShowscoresHeld)
		return true;

	// No esta activado = no dibujar en pantalla
	if (!m_fOn)
		return true;

	const auto width = gEngfuncs.pfnSPR_Width(m_hudSprite, 0);
	const auto height = gEngfuncs.pfnSPR_Height(m_hudSprite, 0);

	//gEngfuncs.pfnSPR_Set(m_hudSprite, 255, 255, 255);
	SPR_Set(m_hudSprite, RGB_WHITE);

	Rect drawingRect;
	drawingRect.left = 0;
	drawingRect.right = width;
	drawingRect.top = 0;
	drawingRect.bottom = height;

	//gEngfuncs.pfnSPR_DrawAdditive(0, (ScreenWidth / 2) - width / 2, (ScreenHeight / 2) - height / 2, &drawingRect);
	SPR_Draw(0, (ScreenWidth / 2) - width / 2, (ScreenHeight / 2) - height / 2, &drawingRect);

	return true;
}

void CHudInventory::MsgFunc_Inventory(const char* pszName, BufferReader& reader)
{
	int x = reader.ReadByte();
	m_fOn = 0 != x;
}