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

#include "cbase.h"
#include "CMinecraftSurvival.h"
#include "CHalfLifeCoopplay.h"

CMinecraftSurvival::CMinecraftSurvival()
{
	CHalfLifeCoopplay::CHalfLifeCoopplay();
}

void CMinecraftSurvival::UpdateGameMode( CBasePlayer* pPlayer ) {
	CHalfLifeCoopplay::UpdateGameMode( pPlayer );
}

void CMinecraftSurvival::MonsterKilled( CBaseMonster* pVictim, CBaseEntity* pKiller, CBaseEntity* inflictor ) {
	CHalfLifeCoopplay::MonsterKilled( pVictim, pKiller, inflictor );
}

bool CMinecraftSurvival::ShouldAutoAim(CBasePlayer* pPlayer, CBaseEntity* target) {
	return CHalfLifeCoopplay::ShouldAutoAim( pPlayer, target );
}

int CMinecraftSurvival::IPointsForKill(CBasePlayer* pAttacker, CBasePlayer* pKilled) {
	CHalfLifeCoopplay::IPointsForKill( pAttacker, pKilled );
}

int CMinecraftSurvival::IPointsForMonsterKill(CBasePlayer* pAttacker, CBaseMonster* pKilled) {
	return CHalfLifeCoopplay::IPointsForMonsterKill( pAttacker, pKilled );
}

float CMinecraftSurvival::FlPlayerSpawnTime(CBasePlayer* pPlayer) {
	return CHalfLifeCoopplay::FlPlayerSpawnTime( pPlayer );
}

void CMinecraftSurvival::PlayerKilled(CBasePlayer* pVictim, CBaseEntity* pKiller, CBaseEntity* inflictor) {
	CHalfLifeCoopplay::PlayerKilled( pVictim, pKiller, inflictor );
}

bool CMinecraftSurvival::FPlayerCanTakeDamage(CBasePlayer* pPlayer, CBaseEntity* pAttacker) {
	CHalfLifeCoopplay::FPlayerCanTakeDamage( pPlayer, pAttacker );
}

int CMinecraftSurvival::PlayerRelationship(CBasePlayer* pPlayer, CBaseEntity* pTarget)
{
	return GR_TEAMMATE;
}

int CMinecraftSurvival::DeadPlayerWeapons(CBasePlayer* pPlayer)
{
	return GR_PLR_DROP_GUN_NO;
}

void CMinecraftSurvival::Think()
{
	if (g_fGameOver)
		CHalfLifeMultiplay::Think();
}
