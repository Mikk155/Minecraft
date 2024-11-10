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
	//m_inventory = new std::vector<CInventory>(static_cast<int>(InventorySlot::Arrows) + 1);
	//m_pInventoryMenu = nullptr;
}

CInventoryMenu::~CInventoryMenu()
{
	delete m_pInventoryMenu;
	m_pInventoryMenu = nullptr;

	delete m_pLocalLabel;
	m_pLocalLabel = nullptr;

	delete m_pButtonSelected;
	m_pButtonSelected = nullptr;
}

int CInventoryMenu::Init(vgui::Panel** pParentPanel)
{
	gHUD.AddHudElem(this);
	m_iFlags |= HUD_ACTIVE;
	m_fOn = false;
	m_pParentPanel = pParentPanel;
	m_pLocalLabel = new vgui::ImagePanel(nullptr);
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

	int idx = 0, idy = 0;
	int start_x = 16, start_y = 16;
	int size_x = 32, size_y = 32;
	int space_x = 4, space_y = 4;

	if (ScreenWidth < 640) 
	{
		if (m_pInventoryMenu = vgui_LoadTGA("gfx/vgui/320_inventory.tga", false); m_pInventoryMenu)
			m_pInventoryMenu->setColor(vgui::Color(255, 255, 255, 1));

		m_Inventory = gHUD.GetSpriteIndex("320inventory");

		start_x /= 2;
		start_y /= 2;
		size_x /= 2;
		size_y /= 2;
		space_x /= 2;
		space_y /= 2;
	}
	else
	{
		if (m_pInventoryMenu = vgui_LoadTGA("gfx/vgui/640_inventory.tga", false); m_pInventoryMenu)
			m_pInventoryMenu->setColor(vgui::Color(255, 255, 255, 1));

		m_Inventory = gHUD.GetSpriteIndex("640inventory");
	}

	//Obtengo el alto y largo de hud.json 
	Rect rect = gHUD.GetSpriteRect(m_Inventory);
	m_pLocalLabel->setVisible(false);
	m_pLocalLabel->setParent(*m_pParentPanel);
	m_pLocalLabel->setImage(m_pInventoryMenu);
	m_pLocalLabel->setBounds((ScreenWidth / 2) - rect.right / 2, (ScreenHeight / 2) - rect.bottom / 2, rect.right, rect.bottom);

	auto createButton = [](int posX, int posY, int sizeX, int sizeY, int x, int y, vgui::Panel** parentPanel, std::vector<InventoryButton*>& m_pButtons)
	{
		vgui::Button* m_pButton = new CommandButton("", posX, posY, sizeX, sizeY);
		m_pButton->setVisible(false);
		m_pButton->setParent(*parentPanel);
		m_pButton->setFgColor(Scheme::sc_primary1);
		m_pButton->setBgColor(0, 0, 0, 0);

		m_pButtons.push_back(new InventoryButton(x, y, m_pButton));
	};

	for (int y = 0; y < 4; y++)
	{
		createButton(((ScreenWidth / 2) - rect.right / 2) + start_x, ((ScreenHeight / 2) - rect.bottom / 2) + start_y, size_x, size_y, idx, idy, m_pParentPanel, m_pButtons);
		start_y += size_y + space_y;
		idy++;
	}

	start_y += space_y * 2;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x <= 8; x++)
		{
			createButton(((ScreenWidth / 2) - rect.right / 2) + start_x + (size_x + space_x) * x, ((ScreenHeight / 2) - rect.bottom / 2) + start_y, size_x, size_y, idx, idy, m_pParentPanel, m_pButtons);
			idx++;
		}

		start_y += size_y + space_y;
		idy++;
		idx = 0;
	}

	start_y += space_y * 2;

	for (int x = 0; x <= 8; x++)
	{
		createButton(((ScreenWidth / 2) - rect.right / 2) + start_x + (size_x + space_x) * x, ((ScreenHeight / 2) - rect.bottom / 2) + start_y, size_x, size_y, idx, idy, m_pParentPanel, m_pButtons);
		idx++;
	}

	return true;
}

bool CInventoryMenu::Draw(float flTime)
{
	if (!m_pLocalLabel)
		return true;

	m_pLocalLabel->setVisible(m_fOn);

	for (auto& pInvButton : m_pButtons)
	{
		pInvButton->m_pButton->setVisible(m_fOn);

		if (pInvButton->m_pButton->isSelected() && pInvButton->m_pButton->wasMousePressed(MOUSE_LEFT))
		{
			pInvButton->m_pButton->setSelected(false);
			pInvButton->m_pButton->setArmed(false);

			// Alterna entre las dos selecciones
			if (!m_pButtonSelected)
			{
				m_pButtonSelected = pInvButton; // Primer botón seleccionado
			}
			else
			{
				// Segundo botón seleccionado
				const auto msg = fmt::format("echo swap {} {} to {} {} \n", m_pButtonSelected->x, m_pButtonSelected->y, pInvButton->x, pInvButton->y);
				gEngfuncs.pfnClientCmd(msg.c_str());

				// Reinicia para una nueva selección después de ejecutar el comando
				m_pButtonSelected = nullptr;
			}
		}
	}


	return true;
}

void CInventoryMenu::FreeBitmaps()
{
	delete m_pInventoryMenu;
	m_pInventoryMenu = nullptr;

	if (m_pLocalLabel)
		m_pLocalLabel->setImage(nullptr);
}

void CInventoryMenu::MsgFunc_Inventory(const char* pszName, BufferReader& reader)
{
	InventoryNetwork action = static_cast<InventoryNetwork>( reader.ReadByte() );

	switch( action )
	{
		case InventoryNetwork::Close:
		{
			// -MC Restore m_inventory objects
			g_iVisibleMouse = m_fOn = true;
			vgui::App::getInstance()->setCursorOveride(vgui::App::getInstance()->getScheme()->getCursor(vgui::Scheme::scu_arrow));
			break;
		}
		case InventoryNetwork::Item:
		{
			int index = reader.ReadByte();
			m_inventory.at(index)->amount = reader.ReadByte();
			m_inventory.at(index)->classname = reader.ReadString();
			break;
		}
		case InventoryNetwork::Data:
		{
			int index = reader.ReadByte();
			int level = reader.ReadByte();
			const char* name = reader.ReadString();
			m_inventory.at(index)->enchants.push_back(g_Minecraft.format_level(name, level));
			break;
		}
		case InventoryNetwork::Open:
		{
			for( size_t i = 0; i < m_inventory.size(); ++i )
			{
				if (auto name = m_inventory.at(i)->classname; name != nullptr)
				{
					// i el slot del inventario
					// name el nombre de tu estructura de informacion
					// Que pueda contener el sprite y display name del item

					// m_inventory->at(i).enchants < encantamientos, si hay alguno
					// m_inventory->at(i).amount < cantidad de items
				}
			}
			g_iVisibleMouse = m_fOn = false;

			//reset
			for (auto& pInvButton : m_pButtons)
			{
				pInvButton->m_pButton->setSelected(false);
				pInvButton->m_pButton->setArmed(false);
			}

			IN_ResetMouse();
			vgui::App::getInstance()->setCursorOveride(vgui::App::getInstance()->getScheme()->getCursor(vgui::Scheme::scu_none));
			break;
		}
	}

	m_pButtonSelected = nullptr;
}