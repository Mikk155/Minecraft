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
