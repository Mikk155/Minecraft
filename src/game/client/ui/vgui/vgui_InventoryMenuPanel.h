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
#include <fmt/format.h>

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

struct InventoryButton
{
	int x, y;
	vgui::Button* m_pButton;

};

struct InventorySize
{
	std::string path; 
	std::string resolution;
	int wide, tall;
	int dx, dy;

	bool resize(int mult)
	{
		if (mult % 320 == 0)
		{
			int factor = std::stoi(resolution) / mult;
			wide /= factor;
			tall /= factor;
			resolution = std::to_string(mult);
		}
		else
		{
			wide *= mult;
			tall *= mult;
			resolution = std::to_string(std::stoi(resolution) * mult);
		}

		return true;
	}

	const char* getPath()
	{
		return fmt::format(path.c_str(), resolution).c_str();
	}
};

class CInventoryMenu : public CHudBase, public vgui::CDefaultInputSignal
{
public:
	CInventoryMenu();
	virtual ~CInventoryMenu();

public:
	virtual int Init(vgui::Panel** pParentPanel);;
	bool VidInit() override;
	bool Draw(float flTime) override;
	void Reset() override;

public:
	void FreeBitmaps();
	InventoryButton* CreateButton(int posX, int posY, int sizeX, int sizeY, int x, int y, vgui::Panel** parentPanel);
	void MsgFunc_Inventory(const char* pszName, BufferReader& reader);

private:
	std::vector<CInventory*> m_inventory; //-M

	vgui::BitmapTGA* m_pInventoryLeft,* m_pInventoryRight;
	vgui::ImagePanel* m_pLocalLabel;
	vgui::Panel** m_pParentPanel;

	std::vector<InventoryButton*> m_pButtons;
	InventoryButton *m_pButtonSelected;
	bool m_fOn;

	int m_Inventory;
};

CInventoryMenu* GetClientInventoryMenu();