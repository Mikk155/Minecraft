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
// vgui_InventoryMenuPanel.cpp
//
#include "hud.h"
#include "vgui_TeamFortressViewport.h"

#include <vgui_loadtga.h>
#include <VGUI_App.h>
#include <VGUI_ButtonGroup.h>
#include <VGUI_RadioButton.h>
#include <VGUI_TextImage.h>

#include <fmt/format.h>

void IN_ResetMouse();
extern bool g_iVisibleMouse;

CInventoryMenu g_InventoryMenu;

CInventoryMenu* GetClientInventoryMenu()
{
	return &g_InventoryMenu;
}

CInventoryMenu::CInventoryMenu()
{
	m_pInventoryLeft = nullptr;
	m_pInventoryRight = nullptr;
}

CInventoryMenu::~CInventoryMenu()
{
	delete m_pInventoryLeft, m_pInventoryRight;
	m_pInventoryLeft = nullptr;
	m_pInventoryRight = nullptr;

	delete m_pLabelInventoryLeft, m_pLabelInventoryRight;
	m_pLabelInventoryLeft = nullptr;
	m_pLabelInventoryRight = nullptr;

	delete m_pButtonSelected;
	m_pButtonSelected = nullptr;
}

int CInventoryMenu::Init(vgui::Panel** pParentPanel)
{
	gHUD.AddHudElem(this);
	m_iFlags |= HUD_ACTIVE;
	m_fOn = false;
	m_pParentPanel = pParentPanel;
	m_pLabelInventoryLeft = new vgui::ImagePanel(nullptr);
	m_pLabelInventoryRight = new vgui::ImagePanel(nullptr);
	m_pButtonSelected = nullptr;

	g_ClientUserMessages.RegisterHandler("Inventory", &CInventoryMenu::MsgFunc_Inventory, this);

	return 1;
}

void CInventoryMenu::Reset()
{
	m_fOn = false;
	m_pButtonSelected = nullptr;
}

bool CInventoryMenu::VidInit()
{
	CInventoryMenu::FreeBitmaps();

	//InventorySize(path, fileName, resolution, size[2], start[2], space, borderThickness)
	InventorySize invSizeLeft{"gfx/vgui", "inventory_left.tga", "640", {166, 214}, {14, 14}, 6, 2};
	InventorySize invSizeRight{"gfx/vgui", "inventory_right.tga", "640", {352, 198}, {14, 14}, 26, 2};
	int button[2] = {36, 36};

	if (ScreenWidth < 640)
	{
		invSizeLeft.resize(320);
		invSizeRight.resize(320);
	}

	if (m_pInventoryLeft = vgui_LoadTGA(invSizeLeft.getPath().c_str(), false); m_pInventoryLeft)
		m_pInventoryLeft->setColor(vgui::Color(255, 255, 255, 1));

	if (m_pInventoryRight = vgui_LoadTGA(invSizeRight.getPath().c_str(), false); m_pInventoryRight)
		m_pInventoryRight->setColor(vgui::Color(255, 255, 255, 1));

	//Mid
	int MidLeft[2] = {invSizeLeft.getMidWide(ScreenWidth, 0, -invSizeRight.getWide()), invSizeLeft.getMidTall(ScreenHeight)};
	int MidRight[2] = {invSizeRight.getMidWide(ScreenWidth, 0, invSizeLeft.getWide()), invSizeRight.getMidTall(ScreenHeight)};

	//Parte izq del inventario
	m_pLabelInventoryLeft->setVisible(false);
	m_pLabelInventoryLeft->setParent(*m_pParentPanel);
	m_pLabelInventoryLeft->setImage(m_pInventoryLeft);
	m_pLabelInventoryLeft->setBounds(MidLeft[0], MidLeft[1], invSizeLeft.getWide(), invSizeLeft.getTall());
	//m_pLabelInventoryLeft->setBounds(invSizeLeft.getMidWide(ScreenWidth), invSizeLeft.getMidTall(ScreenHeight), invSizeLeft.getWide(), invSizeLeft.getTall());

	//Parte der del inventario
	m_pLabelInventoryRight->setVisible(false);
	m_pLabelInventoryRight->setParent(*m_pParentPanel);
	m_pLabelInventoryRight->setImage(m_pInventoryRight);
	m_pLabelInventoryRight->setBounds(MidRight[0], MidRight[1], invSizeRight.getWide(), invSizeRight.getTall());
	//m_pLabelInventoryRight->setBounds(invSizeRight.getMidWide(ScreenWidth), invSizeRight.getMidTall(ScreenHeight), invSizeRight.getWide(), invSizeRight.getTall());

	//Left Side Config
	InitButtonsLeftSide(invSizeLeft, MidLeft, button);

	// Right Side Config
	InitButtonsRightSide(invSizeRight, MidRight, button);

	return true;
}

void CInventoryMenu::InitButtonsLeftSide(InventorySize invSizeLeft, int MidLeft[2], int button[2])
{
	int idButton = 1;

	for (int y = 0; y < 4; y++)
	{
		m_pButtons.push_back(CreateButton(MidLeft[0] + invSizeLeft.getStartX(), MidLeft[1] + invSizeLeft.getStartY(), button[0], button[1], invSizeLeft.getBorderThickness(), idButton++));
		invSizeLeft.start[1] += button[1];
	}

	invSizeLeft.start[1] += invSizeLeft.getSpace();

	for (int x = 0; x < 2; x++)
	{
		m_pButtons.push_back(CreateButton(MidLeft[0] + invSizeLeft.getStartX(), MidLeft[1] + invSizeLeft.getStartY(), button[0], button[1], invSizeLeft.getBorderThickness(), idButton++));
		invSizeLeft.start[0] += button[0];
	}
}

void CInventoryMenu::InitButtonsRightSide(InventorySize invSizeRight, int MidRight[2], int button[2])
{
	int idButton = 1;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			m_pButtons.push_back(CreateButton(MidRight[0] + invSizeRight.getStartX(), MidRight[1] + invSizeRight.getStartY(), button[0], button[1], invSizeRight.getBorderThickness(), idButton++));	
			invSizeRight.start[0] += button[0];
		}

		invSizeRight.start[1] += button[1];
		invSizeRight.start[0] -= button[0] * 9;
	}

	invSizeRight.start[1] += invSizeRight.getSpace();

	for (int x = 0; x < 9; x++)
	{
		m_pButtons.push_back(CreateButton(MidRight[0] + invSizeRight.getStartX(), MidRight[1] + invSizeRight.getStartY(), button[0], button[1], invSizeRight.getBorderThickness(), idButton++));
		invSizeRight.start[0] += button[0];
	}
}

bool CInventoryMenu::Draw(float flTime)
{
	if (!m_pLabelInventoryLeft || !m_pLabelInventoryRight)
		return true;

	m_pLabelInventoryLeft->setVisible(m_fOn);
	m_pLabelInventoryRight->setVisible(m_fOn);

	for (auto& pInvButton : m_pButtons)
	{
		pInvButton->setVisible(m_fOn);

		if (pInvButton->isArmed())
		{
			if (pInvButton->wasMousePressed(MOUSE_LEFT))
			{
				pInvButton->setSelected(false);
				pInvButton->setArmed(false);

				if (m_pButtonSelected)
				{
					const auto msg = fmt::format("echo swap {} to {} \n", m_pButtonSelected->getSlot(), pInvButton->getSlot());
					gEngfuncs.pfnClientCmd(msg.c_str());
				}

				m_pButtonSelected = (m_pButtonSelected) ? nullptr : pInvButton;
			}
		}
	}

	return true;
}

InventoryButton* CInventoryMenu::CreateButton(int posX, int posY, int sizeX, int sizeY, int borderThickness, int slot)
{
	InventoryButton* m_pButton = new InventoryButton(posX, posY, sizeX, sizeY, borderThickness, slot);
	m_pButton->setVisible(false);
	m_pButton->setParent(*m_pParentPanel);

	return m_pButton;
}

void CInventoryMenu::FreeBitmaps()
{
	delete m_pInventoryLeft, m_pInventoryRight;
	m_pInventoryLeft = nullptr;
	m_pInventoryRight = nullptr;

	if (m_pLabelInventoryLeft)
		m_pLabelInventoryLeft->setImage(nullptr);	
	
	if (m_pLabelInventoryRight)
		m_pLabelInventoryRight->setImage(nullptr);
}

void CInventoryMenu::MsgFunc_Inventory(const char* pszName, BufferReader& reader)
{
	InventoryNetwork action = static_cast<InventoryNetwork>( reader.ReadByte() );

	switch( action )
	{
		case InventoryNetwork::Close:
		{
			// Aca limpia todos los objetos
			g_iVisibleMouse = m_fOn = true;
			vgui::App::getInstance()->setCursorOveride(vgui::App::getInstance()->getScheme()->getCursor(vgui::Scheme::scu_arrow));
			break;
		}
		case InventoryNetwork::Item:
		{
			int index = reader.ReadByte();
//			Cantidad de objetos reader.ReadByte();
//			Nombre del objeto reader.ReadString();
			break;
		}
		case InventoryNetwork::Data:
		{
			int index = reader.ReadByte();
			int level = reader.ReadByte();
			const char* name = reader.ReadString();
//			Encantamientos en este item g_Minecraft.format_level(name, level)
			break;
		}
		case InventoryNetwork::Open:
		{
			/* Aca lee los objetos que recibiste */

			g_iVisibleMouse = m_fOn = false;

			//reset
			for (auto& pInvButton : m_pButtons)
			{
				pInvButton->setSelected(false);
				pInvButton->setArmed(false);
			}

			IN_ResetMouse();
			vgui::App::getInstance()->setCursorOveride(vgui::App::getInstance()->getScheme()->getCursor(vgui::Scheme::scu_none));
			break;
		}
	}

	m_pButtonSelected = nullptr;
}