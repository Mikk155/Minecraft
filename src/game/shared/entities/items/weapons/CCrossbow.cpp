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
#include "CCrossbow.h"
#include "UserMessages.h"

#ifndef CLIENT_DLL

class CCrossbowBolt : public CBaseEntity
{
	DECLARE_CLASS(CCrossbowBolt, CBaseEntity);
	DECLARE_DATAMAP();

public:
	void Spawn() override;
	void Precache() override;
	void BubbleThink();
	void BoltTouch(CBaseEntity* pOther);
	void ExplodeThink();

	static CCrossbowBolt* BoltCreate( CBasePlayerWeapon* pWeapon, CBaseMonster* pOwner, string_t sArrowType = string_t::Null );

	string_t m_type;

private:
	int m_iTrail;
};

BEGIN_DATAMAP(CCrossbowBolt)
DEFINE_FUNCTION(BubbleThink),
	DEFINE_FUNCTION(BoltTouch),
	DEFINE_FUNCTION(ExplodeThink),
	END_DATAMAP();

LINK_ENTITY_TO_CLASS(crossbow_bolt, CCrossbowBolt);

CCrossbowBolt* CCrossbowBolt::BoltCreate( CBasePlayerWeapon* pWeapon, CBaseMonster* pOwner, string_t sArrowType )
{
	// Create a new entity with CCrossbowBolt private data
	CCrossbowBolt* pBolt = g_EntityDictionary->Create<CCrossbowBolt>("crossbow_bolt");

	pBolt->pev->owner = pOwner->edict();
	pBolt->pev->euser1 = pWeapon->edict();
	pBolt->m_type = sArrowType;

	pBolt->Spawn();

	return pBolt;
}

void CCrossbowBolt::Spawn()
{
	Precache();
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_BBOX;

	pev->gravity = 0.5;

	SetModel("models/crossbow_bolt.mdl");

	SetOrigin(pev->origin);
	SetSize(Vector(0, 0, 0), Vector(0, 0, 0));

	SetTouch(&CCrossbowBolt::BoltTouch);
	SetThink(&CCrossbowBolt::BubbleThink);
	pev->nextthink = gpGlobals->time + 0.2;
}

void CCrossbowBolt::Precache()
{
	PrecacheModel("models/crossbow_bolt.mdl");
	PrecacheSound("weapons/xbow_hitbod1.wav");
	PrecacheSound("weapons/xbow_hitbod2.wav");
	PrecacheSound("weapons/xbow_fly1.wav");
	PrecacheSound("weapons/xbow_hit1.wav");
	PrecacheSound("fvox/beep.wav");
	m_iTrail = PrecacheModel("sprites/streak.spr");
}

void CCrossbowBolt::BoltTouch(CBaseEntity* pOther)
{
	SetTouch(nullptr);
	SetThink(nullptr);

	if (0 != pOther->pev->takedamage)
	{
		TraceResult tr = UTIL_GetGlobalTrace();
		auto owner = GetOwner();

		// UNDONE: this needs to call TraceAttack instead
		ClearMultiDamage();

		pOther->TraceAttack(this, GetSkillFloat("arrow"sv), pev->velocity.Normalize(), &tr, DMG_NEVERGIB);

		ApplyMultiDamage(this, owner);

		pev->velocity = Vector(0, 0, 0);
		// play body "thwack" sound
		switch (RANDOM_LONG(0, 1))
		{
		case 0:
			EmitSound(CHAN_BODY, "weapons/xbow_hitbod1.wav", 1, ATTN_NORM);
			break;
		case 1:
			EmitSound(CHAN_BODY, "weapons/xbow_hitbod2.wav", 1, ATTN_NORM);
			break;
		}
	}
	else
	{
		EmitSoundDyn(CHAN_BODY, "weapons/xbow_hit1.wav", RANDOM_FLOAT(0.95, 1.0), ATTN_NORM, 0, 98 + RANDOM_LONG(0, 7));

		SetThink(&CCrossbowBolt::SUB_Remove);
		pev->nextthink = gpGlobals->time; // this will get changed below if the bolt is allowed to stick in what it hit.

		if (pOther->ClassnameIs("worldspawn"))
		{
			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = pev->velocity.Normalize();
			SetOrigin(pev->origin - vecDir * 12);
			pev->angles = UTIL_VecToAngles(vecDir);
			pev->solid = SOLID_NOT;
			pev->movetype = MOVETYPE_FLY;
			pev->velocity = Vector(0, 0, 0);
			pev->avelocity.z = 0;
			pev->angles.z = RANDOM_LONG(0, 360);
			// -MC Set "Touch" so we can pick it up back
			pev->nextthink = gpGlobals->time + 10.0;
		}

		if (UTIL_PointContents(pev->origin) != CONTENTS_WATER)
		{
			UTIL_Sparks(pev->origin);
		}
	}

	if( std::string_view( STRING(m_type) ).find( "arrow_firework" ) == 0 )
	{
		SetThink(&CCrossbowBolt::ExplodeThink);
		pev->nextthink = gpGlobals->time + 0.1;
	}
}

void CCrossbowBolt::BubbleThink()
{
	pev->nextthink = gpGlobals->time + 0.1;

	if (pev->waterlevel == WaterLevel::Dry)
		return;

	UTIL_BubbleTrail(pev->origin - pev->velocity * 0.1, pev->origin, 1);
}

void CCrossbowBolt::ExplodeThink()
{
	int iContents = UTIL_PointContents(pev->origin);
	int iScale;

	pev->dmg = 40;
	iScale = 10;

	UTIL_ExplosionEffect(pev->origin, iContents != CONTENTS_WATER ? g_sModelIndexFireball : g_sModelIndexWExplosion,
		iScale, 15, TE_EXPLFLAG_NONE,
		MSG_PVS, pev->origin);

	auto owner = GetOwner();

	pev->owner = nullptr; // can't traceline attack owner if this is set

	::RadiusDamage(pev->origin, this, owner, pev->dmg, 128, DMG_BLAST | DMG_ALWAYSGIB);

	UTIL_Remove(this);
}
#endif

LINK_ENTITY_TO_CLASS( minecraft_bow, CCrossbow);

void CCrossbow::OnCreate()
{
	CBasePlayerWeapon::OnCreate();
	m_iId = WEAPON_CROSSBOW;
	m_iDefaultAmmo = CROSSBOW_DEFAULT_GIVE;
	m_WorldModel = pev->model = MAKE_STRING("models/w_crossbow.mdl");
}

void CCrossbow::Precache()
{
	CBasePlayerWeapon::Precache();
	PrecacheModel(STRING(m_WorldModel));
	PrecacheModel("models/v_crossbow.mdl");
	PrecacheModel("models/p_crossbow.mdl");

	PrecacheSound("weapons/xbow_fire1.wav");
	PrecacheSound("weapons/xbow_reload1.wav");

	UTIL_PrecacheOther("crossbow_bolt");
}

bool CCrossbow::GetWeaponInfo(WeaponInfo& info)
{
	info.Name = STRING(pev->classname);
	info.AttackModeInfo[0].AmmoType = "bolts";
	info.AttackModeInfo[0].MagazineSize = WEAPON_NOCLIP;
	info.Slot = 2;
	info.Position = 2;
	info.Id = WEAPON_CROSSBOW;
	info.Weight = CROSSBOW_WEIGHT;
	return true;
}

bool CCrossbow::Deploy()
{
	return DefaultDeploy("models/v_crossbow.mdl", "models/p_crossbow.mdl", 0, "bow");
}

void CCrossbow::Holster()
{
	FireArrow();
}

bool CCrossbow::ReloadArrow( int arrows )
{
	if( !FStringNull( m_type ) )
	{
		return true;
	}

#ifndef CLIENT_DLL
	minecraft::inventory* item = &m_pPlayer->inventory[minecraft::slot::ARROWS];

	// Check for other arrows and attempt to equip
	if( item->pItem == nullptr )
	{
		for( size_t i = minecraft::slot::ARROWS + 1; i < m_pPlayer->inventory.size(); ++i )
		{
			minecraft::inventory* pNewItem = &m_pPlayer->inventory[i];

			if( pNewItem->pItem != nullptr && std::string_view( pNewItem->pItem->GetClassname() ).find( "arrow" ) == 0 )
			{
				std::swap( m_pPlayer->inventory[minecraft::slot::ARROWS], m_pPlayer->inventory[i] );
				item = &m_pPlayer->inventory[minecraft::slot::ARROWS];
				break;
			}
		}
	}

	if( item->pItem == nullptr )
	{
		m_type = item->pItem->pev->classname;
		item->amount -= arrows;

		if( item->amount == 0 )
		{
			item->pItem = nullptr;
		}

		return true;
	}
#endif
	return false;
}

void CCrossbow::SecondaryAttack()
{
	if( ReloadArrow() )
	{
		if( float flChargeTime = GetSkillFloat( "player_bow_charge_time"sv ); flCharge < flChargeTime )
		{
			flCharge += 0.1;

		    float flInterpolation = 1 + ( flCharge / flChargeTime ) * ( 7 - 1 );
    		int iAnim = std::clamp( static_cast<int>( std::round( flInterpolation ) ), 1, 7 );
			SendWeaponAnim(iAnim);
		}
	}

	m_flNextSecondaryAttack = m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.1;
}

void CCrossbow::FireArrow()
{
	if( FStringNull( m_type ) )
	{
		flCharge = 0;
		return;
	}

	SendWeaponAnim(0);

	m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;

	int flags;
#if defined(CLIENT_WEAPONS)
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	Vector anglesAim = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors(anglesAim);

	anglesAim.x = -anglesAim.x;

#ifndef CLIENT_DLL
	Vector vecSrc = m_pPlayer->GetGunPosition() - gpGlobals->v_up * 2;
	Vector vecDir = gpGlobals->v_forward;

	CCrossbowBolt* pBolt = CCrossbowBolt::BoltCreate( this, m_pPlayer, m_type );
	pBolt->pev->origin = vecSrc;
	pBolt->pev->angles = anglesAim;

	int ARROW_VELOCITY = GetSkillFloat( "player_bow_distance"sv ) * flCharge;

	if (m_pPlayer->pev->waterlevel == WaterLevel::Head)
	{
		int ARROW_VELOCITY_WATER = ARROW_VELOCITY * GetSkillFloat( "player_bow_distance_water"sv );
		pBolt->pev->velocity = vecDir * ARROW_VELOCITY_WATER;
		pBolt->pev->speed = ARROW_VELOCITY_WATER;
	}
	else
	{
		pBolt->pev->velocity = vecDir * ARROW_VELOCITY;
		pBolt->pev->speed = ARROW_VELOCITY;
	}
	pBolt->pev->avelocity.z = 10;
	pBolt->pev->velocity.z = -GetSkillFloat( "player_bow_decay"sv );
#endif

	flCharge = 0;
	m_type = string_t::Null;

	m_pPlayer->m_flNextAttack =
		m_flNextPrimaryAttack =
			m_flNextSecondaryAttack =
				UTIL_WeaponTimeBase() +
					GetSkillFloat( "player_bow_cooldown_time"sv );
}

void CCrossbow::WeaponIdle()
{
	if( flCharge > 0 && !FBitSet( m_pPlayer->pev->button, IN_ATTACK2 ) )
	{
		FireArrow();
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1;
}
