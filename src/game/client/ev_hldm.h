//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

void EV_HLDM_GunshotDecalTrace(pmtrace_t* pTrace, char* decalName);
void EV_HLDM_DecalGunshot(pmtrace_t* pTrace, int iBulletType);
void EV_HLDM_CheckTracer(int idx, const Vector& vecSrc, const Vector& end, const Vector& forward, const Vector& right,
	int iBulletType, int iTracerFreq, int* tracerCount);
void EV_HLDM_FireBullets(int idx,
	const Vector& forward, const Vector& right, const Vector& up,
	int cShots, const Vector& vecSrc, const Vector& vecDirShooting,
	float flDistance, int iBulletType, int iTracerFreq, int* tracerCount, float flSpreadX, float flSpreadY);

// HLDM

void SetLocalBody(int id, int body);
