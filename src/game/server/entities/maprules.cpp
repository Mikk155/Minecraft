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

/**
 *	@file
 *
 *	This module contains entities for implementing/changing game rules dynamically within each map (.BSP)
 */

#include "cbase.h"
#include "maprules.h"

class CRuleEntity : public CBaseEntity
{
	DECLARE_CLASS(CRuleEntity, CBaseEntity);

public:
	void Spawn() override;

protected:
	bool CanFireForActivator(CBaseEntity* pActivator);
};

void CRuleEntity::Spawn()
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	pev->effects = EF_NODRAW;
}

bool CRuleEntity::CanFireForActivator(CBaseEntity* pActivator)
{
	if (!FStringNull(m_sMaster))
	{
		if (UTIL_IsMasterTriggered(m_sMaster, pActivator))
			return true;
		else
			return false;
	}

	return true;
}

/**
 *	@brief base class for all rule "point" entities (not brushes)
 */
class CRulePointEntity : public CRuleEntity
{
public:
	void Spawn() override;
};

void CRulePointEntity::Spawn()
{
	CRuleEntity::Spawn();
	pev->frame = 0;
	pev->model = string_t::Null;
}

/**
 *	@brief base class for all rule "brush" entities (not brushes)
 *	Default behavior is to set up like a trigger, invisible, but keep the model for volume testing
 */
class CRuleBrushEntity : public CRuleEntity
{
public:
	void Spawn() override;

private:
};

void CRuleBrushEntity::Spawn()
{
	SetModel(STRING(pev->model));
	CRuleEntity::Spawn();
}

/**
 *	@brief Ends the game in MP
 */
class CGameEnd : public CRulePointEntity
{
public:
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;

private:
};

LINK_ENTITY_TO_CLASS(game_end, CGameEnd);

void CGameEnd::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (!CanFireForActivator(pActivator))
		return;

	g_pGameRules->EndMultiplayerGame(true);
}

#define SF_ENVTEXT_ALLPLAYERS 0x0001

/**
 *	@brief NON-Localized HUD Message (use env_message to display a titles.txt message)
 */
class CGameText : public CRulePointEntity
{
	DECLARE_CLASS(CGameText, CRulePointEntity);
	DECLARE_DATAMAP();

public:
	void Precache() override;
	void Spawn() override;

	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
	bool KeyValue(KeyValueData* pkvd) override;

	inline bool MessageToAll() { return (pev->spawnflags & SF_ENVTEXT_ALLPLAYERS) != 0; }
	inline void MessageSet(const char* pMessage) { pev->message = ALLOC_STRING(pMessage); }
	inline const char* MessageGet() { return STRING(pev->message); }

private:
	hudtextparms_t m_textParms;
};

LINK_ENTITY_TO_CLASS(game_text, CGameText);

// Save parms as a block.
// Will break save/restore if the structure changes, but this entity didn't ship with Half-Life,
// so it can't impact saved Half-Life games.
BEGIN_DATAMAP(CGameText)
DEFINE_ARRAY(m_textParms, FIELD_CHARACTER, sizeof(hudtextparms_t)),
	END_DATAMAP();

void CGameText::Precache()
{
	CRulePointEntity::Precache();

	// Re-allocate the message to handle escape characters
	if (!FStringNull(pev->message))
	{
		pev->message = ALLOC_ESCAPED_STRING(STRING(pev->message));
	}
}

void CGameText::Spawn()
{
	Precache();

	CRulePointEntity::Spawn();
}

bool CGameText::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "channel"))
	{
		m_textParms.channel = atoi(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "x"))
	{
		m_textParms.x = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "y"))
	{
		m_textParms.y = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "effect"))
	{
		m_textParms.effect = atoi(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "color"))
	{
		int color[4];
		UTIL_StringToIntArray(color, 4, pkvd->szValue);
		m_textParms.r1 = color[0];
		m_textParms.g1 = color[1];
		m_textParms.b1 = color[2];
		m_textParms.a1 = color[3];
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "color2"))
	{
		int color[4];
		UTIL_StringToIntArray(color, 4, pkvd->szValue);
		m_textParms.r2 = color[0];
		m_textParms.g2 = color[1];
		m_textParms.b2 = color[2];
		m_textParms.a2 = color[3];
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "fadein"))
	{
		m_textParms.fadeinTime = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "fadeout"))
	{
		m_textParms.fadeoutTime = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "holdtime"))
	{
		m_textParms.holdTime = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "fxtime"))
	{
		m_textParms.fxTime = atof(pkvd->szValue);
		return true;
	}

	return CRulePointEntity::KeyValue(pkvd);
}

void CGameText::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (!CanFireForActivator(pActivator))
		return;

	if (MessageToAll())
	{
		UTIL_HudMessageAll(m_textParms, MessageGet());
	}
	else
	{
		if (auto player = ToBasePlayer(pActivator); player && player->IsNetClient())
		{
			UTIL_HudMessage(player, m_textParms, MessageGet());
		}
	}
}

#define SF_GAMECOUNT_FIREONCE 0x0001
#define SF_GAMECOUNT_RESET 0x0002

/**
 *	@brief Counts events and fires target
 *	@details Flag: Fire once
 *	Flag: Reset on Fire
 */
class CGameCounter : public CRulePointEntity
{
public:
	void Spawn() override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
	inline bool RemoveOnFire() { return (pev->spawnflags & SF_GAMECOUNT_FIREONCE) != 0; }
	inline bool ResetOnFire() { return (pev->spawnflags & SF_GAMECOUNT_RESET) != 0; }

	inline void CountUp() { pev->frags++; }
	inline void CountDown() { pev->frags--; }
	inline void ResetCount() { pev->frags = pev->dmg; }
	inline int CountValue() { return pev->frags; }
	inline int LimitValue() { return pev->health; }

	inline bool HitLimit() { return CountValue() == LimitValue(); }

private:
	inline void SetCountValue(int value) { pev->frags = value; }
	inline void SetInitialValue(int value) { pev->dmg = value; }
};

LINK_ENTITY_TO_CLASS(game_counter, CGameCounter);

void CGameCounter::Spawn()
{
	// Save off the initial count
	SetInitialValue(CountValue());
	CRulePointEntity::Spawn();
}

void CGameCounter::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (!CanFireForActivator(pActivator))
		return;

	switch (useType)
	{
	case USE_ON:
	case USE_TOGGLE:
		CountUp();
		break;

	case USE_OFF:
		CountDown();
		break;

	case USE_SET:
		SetCountValue((int)value);
		break;
	}

	if (HitLimit())
	{
		SUB_UseTargets(pActivator, USE_TOGGLE, 0);
		if (RemoveOnFire())
		{
			UTIL_Remove(this);
		}

		if (ResetOnFire())
		{
			ResetCount();
		}
	}
}

#define SF_PLAYEREQUIP_USEONLY 0x0001
#define MAX_EQUIP 32

/**
 *	@brief Sets the default player equipment
 *	Flag: USE Only
 */
class CGamePlayerEquip : public CRulePointEntity
{
	DECLARE_CLASS(CGamePlayerEquip, CRulePointEntity);
	DECLARE_DATAMAP();

public:
	bool KeyValue(KeyValueData* pkvd) override;
	void Touch(CBaseEntity* pOther) override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;

	inline bool UseOnly() { return (pev->spawnflags & SF_PLAYEREQUIP_USEONLY) != 0; }

private:
	void EquipPlayer(CBaseEntity* pPlayer);

	string_t m_weaponNames[MAX_EQUIP];
	int m_weaponCount[MAX_EQUIP];
};

BEGIN_DATAMAP(CGamePlayerEquip)
DEFINE_ARRAY(m_weaponNames, FIELD_STRING, MAX_EQUIP),
	DEFINE_ARRAY(m_weaponCount, FIELD_INTEGER, MAX_EQUIP),
	END_DATAMAP();

LINK_ENTITY_TO_CLASS(game_player_equip, CGamePlayerEquip);

bool CGamePlayerEquip::KeyValue(KeyValueData* pkvd)
{
	if (CRulePointEntity::KeyValue(pkvd))
	{
		return true;
	}

	for (int i = 0; i < MAX_EQUIP; i++)
	{
		if (FStringNull(m_weaponNames[i]))
		{
			char tmp[128];

			UTIL_StripToken(pkvd->szKeyName, tmp);

			m_weaponNames[i] = ALLOC_STRING(tmp);
			m_weaponCount[i] = atoi(pkvd->szValue);
			m_weaponCount[i] = std::max(1, m_weaponCount[i]);
			return true;
		}
	}

	return false;
}

void CGamePlayerEquip::Touch(CBaseEntity* pOther)
{
	if (!CanFireForActivator(pOther))
		return;

	if (UseOnly())
		return;

	EquipPlayer(pOther);
}

void CGamePlayerEquip::EquipPlayer(CBaseEntity* pEntity)
{
	auto player = ToBasePlayer(pEntity);

	if (!player)
	{
		return;
	}

	for (int i = 0; i < MAX_EQUIP; i++)
	{
		if (FStringNull(m_weaponNames[i]))
			break;
		for (int j = 0; j < m_weaponCount[i]; j++)
		{
			player->GiveNamedItem(STRING(m_weaponNames[i]));
		}
	}
}

void CGamePlayerEquip::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	EquipPlayer(pActivator);
}
