//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include "VGUI_Label.h"
#include "VGUI_LineBorder.h"
#include "VGUI_ImagePanel.h"
#include "VGUI_BitmapTGA.h"
#include "VGUI_InputSignal.h"
#include "VGUI_Button.h"
#include "cl_entity.h"
#include "vgui_checkbutton2.h"
#include "vgui_defaultinputsignal.h"

#include "CMinecraft.h"

class CInventoryMenu;
class BufferReader;

class CInventoryMenu : public CHudBase, public vgui::CDefaultInputSignal
{
public:
	CInventoryMenu();
	virtual ~CInventoryMenu();

	// CHudBase overrides.
public:
	virtual int Init(vgui::Panel** pParentPanel);;
	bool VidInit() override;
	bool Draw(float flTime) override;
	void Reset() override;

public:
	void FreeBitmaps();
	void MsgFunc_Inventory(const char* pszName, BufferReader& reader);

private:
	std::vector<CInventory>* m_inventory;
	vgui::BitmapTGA* m_pInventoryMenu;
	vgui::ImagePanel* m_pLocalLabel;
	vgui::Panel** m_pParentPanel;
	bool m_fOn;
};

// Get the (global) voice manager.
CInventoryMenu* GetClientInventoryMenu();