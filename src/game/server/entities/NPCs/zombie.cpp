/***
 *
 *	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   This source code contains proprietary and confidential information of
 *   Valve LLC and its suppliers.  Access to this code is restricted to
 *   persons who have executed a written SDK license with Valve.  Any access,
 *   use or distribution of this code by or to any unlicensed person is illegal.
 *
 ****/

#include "cbase.h"

enum ZOMBIE_SKIN
{
	ZOMBIE_NORMAL = 0,
	ZOMBIE_DROWNED_OUT_LAYER,
	ZOMBIE_VILLAGER,
	ZOMBIE_DROWNED,
	ZOMBIE_HUSK
};

enum ZOMBIE_ANIMATIONS
{
	ZOMBIE_ATTACK_RIGHT = 0x01,
	ZOMBIE_ATTACK_LEFT = 0x02,
	ZOMBIE_ATTACK_BOTH = 0x03,

};

class CZombie : public CBaseMonster
{
public:
	void OnCreate() override;
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	int IgnoreConditions() override;

	bool HasAlienGibs() override;

	//Sonidos
	void PainSound() override;
	void IdleSound() override;
	void AttackSound();

	//No hay ataques a larga distancia
	bool CheckRangeAttack1(float flDot, float flDist) override;
	bool CheckRangeAttack2(float flDot, float flDist) override;
	bool TakeDamage(CBaseEntity* inflictor, CBaseEntity* attacker, float flDamage, int bitsDamageType) override;

protected:
	void ZombieSlashAttack(float damage, const Vector& punchAngle, const Vector& velocity);
};

LINK_ENTITY_TO_CLASS(monster_zombie, CZombie);

void CZombie::OnCreate()
{
	CBaseMonster::OnCreate();

	pev->health = 100;
	pev->model = MAKE_STRING("models/zombie.mdl");

	SetClassification("alien_monster");
}

void CZombie::SetYawSpeed()
{
	pev->yaw_speed = 120;
}

bool CZombie::HasAlienGibs()
{ 
	return true; 
}

bool CZombie::CheckRangeAttack1(float flDot, float flDist)
{
	return false;
}

bool CZombie::CheckRangeAttack2(float flDot, float flDist)
{
	return false;
}

bool CZombie::TakeDamage(CBaseEntity* inflictor, CBaseEntity* attacker, float flDamage, int bitsDamageType)
{
	if (IsAlive())
		PainSound();

	return CBaseMonster::TakeDamage(inflictor, attacker, flDamage, bitsDamageType);
}

void CZombie::PainSound()
{
	/*	
		int pitch = 95 + RANDOM_LONG(0, 9);

		if (RANDOM_LONG(0, 5) < 2)
			EmitSoundDyn(CHAN_VOICE, RANDOM_SOUND_ARRAY(pPainSounds), 1.0, ATTN_NORM, 0, pitch);
	*/
}

void CZombie::IdleSound()
{
	/*
		int pitch = 100 + RANDOM_LONG(-5, 5);

		// Play a random idle sound
		EmitSoundDyn(CHAN_VOICE, RANDOM_SOUND_ARRAY(pIdleSounds), 1.0, ATTN_NORM, 0, pitch);
	*/
}

void CZombie::AttackSound()
{
	/*
		int pitch = 100 + RANDOM_LONG(-5, 5);

		// Play a random attack sound
		EmitSoundDyn(CHAN_VOICE, RANDOM_SOUND_ARRAY(pAttackSounds), 1.0, ATTN_NORM, 0, pitch);
	*/
}

void CZombie::ZombieSlashAttack(float damage, const Vector& punchAngle, const Vector& velocity)
{
	CBaseEntity* pHurt = CheckTraceHullAttack(70, damage, DMG_SLASH);

	if (pHurt)
	{
		if ((pHurt->pev->flags & (FL_MONSTER | FL_CLIENT)) != 0)
		{
			pHurt->pev->punchangle = punchAngle;
			pHurt->pev->velocity = pHurt->pev->velocity + velocity;
		}
	}

	AttackSound();
}


void CZombie::HandleAnimEvent(MonsterEvent_t* pEvent)
{
	float SlashDamage = 25;

	switch (pEvent->event)
	{
		case ZOMBIE_ATTACK_RIGHT:
		ZombieSlashAttack(SlashDamage, {5, 0, -18}, -(gpGlobals->v_right * 100));
			break;

		case ZOMBIE_ATTACK_LEFT:
			ZombieSlashAttack(SlashDamage, {5, 0, 18}, gpGlobals->v_right * 100);
			break;

		case ZOMBIE_ATTACK_BOTH:
			ZombieSlashAttack(SlashDamage * 2, {5, 0, 0}, gpGlobals->v_forward * -100);
			break;

		default:
			CBaseMonster::HandleAnimEvent(pEvent);
			break;
	}
}

void CZombie::Spawn()
{
	Precache();

	SetModel(STRING(pev->model));
	SetSize(VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = DONT_BLEED;
	pev->view_ofs = VEC_VIEW; // position of the eyes relative to monster's origin.
	m_flFieldOfView = 0.5;	  // indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState = MONSTERSTATE_NONE;
	m_afCapability = bits_CAP_DOORS_GROUP;

	MonsterInit();
}

void CZombie::Precache()
{
	PrecacheModel(STRING(pev->model));
}

int CZombie::IgnoreConditions()
{
	int iIgnore = CBaseMonster::IgnoreConditions();

	return iIgnore;
}