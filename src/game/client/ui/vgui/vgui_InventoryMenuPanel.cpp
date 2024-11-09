//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

// There are hud.h's coming out of the woodwork so this ensures that we get the right one.
#include "hud.h"

#include "demo.h"
#include "demo_api.h"
#include "vgui_InventoryMenuPanel.h"
#include "r_efx.h"
#include "entity_types.h"
#include "VGUI_ActionSignal.h"
#include "VGUI_Scheme.h"
#include "VGUI_TextImage.h"
#include "vgui_loadtga.h"
#include "VGUI_App.h"

void IN_ResetMouse();
extern bool g_iVisibleMouse;

CInventoryMenu g_InventoryMenu;

CInventoryMenu* GetClientInventoryMenu()
{
	return &g_InventoryMenu;
}

CInventoryMenu::CInventoryMenu()
{
	m_inventory = new std::vector<CInventory>(static_cast<int>(InventorySlot::Arrows) + 1);
	m_pInventoryMenu = nullptr;
}

CInventoryMenu::~CInventoryMenu()
{
	delete m_pInventoryMenu;
	m_pInventoryMenu = nullptr;

	delete m_pLocalLabel;
	m_pLocalLabel = nullptr;

	delete m_inventory;
	m_inventory = nullptr;
}

int CInventoryMenu::Init(vgui::Panel** pParentPanel)
{
	gHUD.AddHudElem(this);
	m_iFlags |= HUD_ACTIVE;
	m_fOn = false;
	m_pParentPanel = pParentPanel;
	m_pLocalLabel = new vgui::ImagePanel(nullptr);

	g_ClientUserMessages.RegisterHandler("Inventory", &CInventoryMenu::MsgFunc_Inventory, this);

	return 1;
}

void CInventoryMenu::Reset()
{
	m_fOn = false;
}

bool CInventoryMenu::VidInit()
{
	CInventoryMenu::FreeBitmaps();

	if (m_pInventoryMenu = vgui_LoadTGA("gfx/vgui/640_inventory.tga", false); m_pInventoryMenu)
		m_pInventoryMenu->setColor(vgui::Color(255, 255, 255, 1));

	return true;
}

bool CInventoryMenu::Draw(float flTime)
{
	if (!m_pLocalLabel)
		return true;

	// don't draw the menu if the scoreboard is being shown
	if (gHUD.m_Scoreboard.m_iShowscoresHeld)
		return true;

	
	int sizeX, sizeY;
	m_pInventoryMenu->getSize(sizeX, sizeY);
	m_pLocalLabel->setParent(*m_pParentPanel);
	m_pLocalLabel->setImage(m_pInventoryMenu);
	m_pLocalLabel->setBounds((ScreenWidth / 2) - sizeX / 2, (ScreenHeight / 2) - sizeY / 2, sizeX, sizeY);
	m_pLocalLabel->setVisible(m_fOn);

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
			g_iVisibleMouse = m_fOn = false;
			vgui::App::getInstance()->setCursorOveride(vgui::App::getInstance()->getScheme()->getCursor(vgui::Scheme::scu_arrow));
			break;
		}
		case InventoryNetwork::Item:
		{
			int index = reader.ReadByte();
			m_inventory->at(index).amount = reader.ReadByte();
			m_inventory->at(index).classname = reader.ReadString();
			break;
		}
		case InventoryNetwork::Data:
		{
			int index = reader.ReadByte();
			int level = reader.ReadByte();
			const char* name = reader.ReadString();
			std::string_view sv = g_Minecraft.format_level( name, level );
			m_inventory->at(index).enchants.push_back( sv );
			break;
		}
		case InventoryNetwork::Open:
		{
			g_iVisibleMouse = m_fOn = true;
			IN_ResetMouse();
			vgui::App::getInstance()->setCursorOveride(vgui::App::getInstance()->getScheme()->getCursor(vgui::Scheme::scu_none));
			break;
		}
	}
}