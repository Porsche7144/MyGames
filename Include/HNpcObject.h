#pragma once

#include "HStandState.h"
#include "HMoveState.h"
#include "HAttackState.h"

class HNpcObject : public HObject
{
public:
	HEffect*						m_pProjectile;
	std::vector<HProjectileInfo>	m_ProjectileList;
	HEnemyState*					m_pAction;
	std::vector<HEnemyState*>  m_pActionList;
public:
	virtual void FSM();
	virtual void SetTransition(DWORD dwEevnt);
public:
	virtual bool Damage()				override;
	virtual bool Dead()					override;
public:
	virtual HObject* Clone() override;
	virtual bool Frame() override;
	virtual bool Render(ID3D11DeviceContext* pd3dContext) override;
	virtual bool Release() override;
	HNpcObject()
	{
		m_iObjectType = 200;
	}
};

