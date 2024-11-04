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

#include "hud.h"

#include "vgui_TeamFortressViewport.h"

void IN_ResetMouse();
extern bool g_iVisibleMouse;

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
	m_HUD_inventory = gHUD.GetSpriteIndex("inventory");

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

	//gEngfuncs.pfnSPR_Set(m_hudSprite, 255, 255, 255);
	SPR_Set(gHUD.GetSprite(m_HUD_inventory), RGB_WHITE);

	Rect rect = gHUD.GetSpriteRect(m_HUD_inventory);
	//gEngfuncs.pfnSPR_DrawAdditive(0, (ScreenWidth / 2) - width / 2, (ScreenHeight / 2) - height / 2, &drawingRect);
	SPR_Draw(0, (ScreenWidth / 2) - rect.right / 2, (ScreenHeight / 2) - rect.bottom / 2, &rect);
	return true;
}

void CHudInventory::MsgFunc_Inventory(const char* pszName, BufferReader& reader)
{
	int x = reader.ReadByte();
	m_fOn = 0 != x;
	g_iVisibleMouse = m_fOn;

	if (g_iVisibleMouse)
	{
		App::getInstance()->setCursorOveride(App::getInstance()->getScheme()->getCursor(Scheme::scu_arrow));
	}
	else
	{
		IN_ResetMouse();
		App::getInstance()->setCursorOveride(App::getInstance()->getScheme()->getCursor(Scheme::scu_none));
	}
}