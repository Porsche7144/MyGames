#pragma once
#include "HProjectile.h"

class HGameUser : public HObject
{
public:
	bool		m_bAttack;	
	float		m_fKeyTimer;
	HPoint		m_tTargetPos;
	DWORD		m_dwMove;
	DWORD		m_dwPreMove;
	HEffect*    m_pProjectile;
	std::vector<HProjectileInfo>	m_ProjectileList;
public:
	bool Reset() override;
	bool Init() override;
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pd3dContext) override;
public:
	virtual HObject* Clone() override;
	virtual bool Damage();
	virtual bool Dead();
	virtual bool Hit(HPoint p, int iIndex);
	virtual bool ProjectileTick();
	HGameUser()
	{
		m_dwSvrID = 0;
		m_iObjectType = 100;
		m_fSpeed = 300.0f;
	}
};

