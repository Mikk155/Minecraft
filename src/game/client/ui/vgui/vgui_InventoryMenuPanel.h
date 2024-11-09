//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include "vector"
#include "VGUI_Label.h"
#include "VGUI_ImagePanel.h"
#include "VGUI_BitmapTGA.h"

#include "CMinecraft.h"

class CInventoryMenu;
class BufferReader;

namespace vgui
{
	class Button;
	class ButtonGroup;
	class CListBox;
	class RadioButton;
	class TextPanel;
}

class InventoryButton
{
public:
	int x, y;
	vgui::Button* m_pButton;

	InventoryButton(int x, int y, vgui::Button* m_pButton)
		: x(x), y(y), m_pButton(m_pButton)
	{
	}
};

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

	std::vector<InventoryButton*> m_pButtons;
	bool m_fOn;
	int m_Inventory;
};

// Get the (global) voice manager.
CInventoryMenu* GetClientInventoryMenu();