/***
 *
 *	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
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

#pragma once

/**
*	@brief Inventory networking enum used for gmsgInventory
*/
enum class InventoryNetwork : int
{
    /**
    *	@brief Closes the inventory.
    *   @param byte ``InventoryNetwork::Close``
    */
	Close = 0,

    /**
    *	@brief Sends a item.
    *   @param byte ``InventoryNetwork::Item``
    *   @param byte Slot where this item is ``InventorySlot``
    *   @param byte Entity index for the given CBaseItem instance.
    */
	Item,

    /**
    *	@brief Sends enchantments for the given item.
    *   @param byte ``InventoryNetwork::Data``
    *   @param byte Slot where this item is ``InventorySlot``
    *   @param string name of the enchant, one of ``g_Minecraft.Effects.Dictionary``
    *   @param byte Enchantment level.
    */
	Data,

    /**
    *	@brief All data sent, Open the inventary.
    *   @param byte ``InventoryNetwork::Open``
    */
	Open
};

// Skin values for upgrade-able items
enum class ItemMaterial : int
{
    /**
    *	@brief Sword and axe only.
    */
	Wood = 0,
    /**
    *	@brief Sword and axe only.
    */
	Stone,
    /**
    *	@brief Armor only.
    */
	Leather = 0,
    /**
    *	@brief Armor only.
    */
	Chainmail,
	Iron,
	Gold,
	Diamond,
	Netherite
};

/**
*	@brief inventory slot enum for ``std::vector<CBaseItem*> CBaseMonster::inventory``.
*/
enum class InventorySlot : int
{
    /**
    *	@brief These can be interacted by the player.
    *   @param Hotbar1 Monsters will use only this
    *   @param LeftHand Monsters will use only this
    */
	LeftHand = 0, Hotbar1, Hotbar2, Hotbar3, Hotbar4, Hotbar5, Hotbar6, Hotbar7, Hotbar8, Hotbar9,

    /**
    *	@brief These automatically reduce damage received.
    */
	ArmorHelmet, ArmorChest, ArmorLegs, ArmorBoots,

    /**
    *	@brief These are automatically used when using a crossbow or bow.
    *   @param Arrows Only projectiles can be located in this slot.
    */
	Arrows,

    /**
    *	@brief Non-interactive slots
    *   @param FirstItemSlot Start of non-interactive slots.
    *   @param LastItemSlot End of non-interactive slots.
    */
	FirstItemSlot = 16, /* ... ... */ LastItemSlot = 42,

    /**
    *	@brief Invalid slot for equiping.
    */
	InvalidAddSlot = 13,

	MAX_SLOTS = 43
};

/**
*	@brief DMG Bits enum.
*/
enum class DMG : int
{
    /**
    *	@brief Generic damage.
    */
	GENERIC	= 0,

    /**
    *	@brief Prevents recursion when using Sweeping Edge.
    */
	SLASH	= ( 1 << 0 ),

    /**
    *	@brief Displays fire on the HUD.
    */
	FIRE	= ( 1 << 1 ),

    /**
    *	@brief Apply effect fire I to the victim
    */
	LAVA	= ( 1 << 2 )
};
