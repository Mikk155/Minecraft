//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include "vector"
#include <fmt/format.h>
#include <VGUI_Button.h>
#include <VGUI_BitmapTGA.h>
#include <VGUI_ImagePanel.h>
#include <VGUI_Scheme.h>

//#include "CMinecraft.h"

class CInventoryMenu;
class BufferReader;
class InventoryButton;

struct InventorySize
{
public:
	std::string path;
	std::string fileName;
	std::string resolution;
	int size[2];
	int start[2];
	int space;
	int borderThickness;

public:
	bool resize(float mult)
	{
		if ((int)mult % 320 == 0)
		{
			int factor = std::stoi(resolution) / mult;
			size[0] /= factor;
			size[1] /= factor;
			start[0] /= factor;
			start[1] /= factor;
			space /= factor;
			borderThickness /= factor;
			resolution = std::to_string(mult);
		}
		else
		{
			size[0] *= mult;
			size[1] *= mult;
			start[0] *= mult;
			start[1] *= mult;
			space *= mult;
			borderThickness *= mult;
			resolution = std::to_string(std::stoi(resolution) * mult);
		}
		return true;
	}

	int getMidWide(int max1, int min1 = 0, int minWide = 0) const
	{
		return ((max1 - min1) / 2) - ((getWide() - minWide) / 2);
	}	
	
	int getMidTall(int max1, int min1 = 0, int minTall = 0) const
	{
		return ((max1 - min1) / 2) - ((getTall() - minTall) / 2);
	}

	const std::string getPath() const
	{
		return fmt::format("{}/{}/{}", path, resolution, fileName);
	}

	int getWide() const 
	{
		return this->size[0]; 
	}

	int getTall() const 
	{ 
		return this->size[1]; 
	}

	int getStartX() const
	{
		return this->start[0];
	}	
	
	int getStartY() const
	{
		return this->start[1];
	}

	int getSpace() const
	{
		return this->space;
	}

	int getBorderThickness() const
	{
		return this->borderThickness;
	}
};

class CInventoryMenu : public CHudBase
{
public:
	CInventoryMenu();
	virtual ~CInventoryMenu();

public:
	virtual int Init(vgui::Panel** pParentPanel); // int Init();
	bool VidInit() override;
	bool Draw(float flTime) override;
	void Reset() override;

public:
	void InitButtonsLeftSide(InventorySize invSizeLeft, int MidLeft[2], int button[2]);
	void InitButtonsRightSide(InventorySize invSizeRight, int MidRight[2], int button[2]);
	void FreeBitmaps();
	InventoryButton* CreateButton(int posX, int posY, int sizeX, int sizeY, int borderThickness, int slot);

public:
	void MsgFunc_Inventory(const char* pszName, BufferReader& reader);

private:
	vgui::BitmapTGA* m_pInventoryLeft,* m_pInventoryRight;
	vgui::ImagePanel* m_pLabelInventoryLeft,* m_pLabelInventoryRight;
	vgui::Panel** m_pParentPanel;

	std::vector<InventoryButton*> m_pButtons;
	InventoryButton* m_pButtonSelected;
	bool m_fOn;

	int m_Inventory;
};

CInventoryMenu* GetClientInventoryMenu();