#pragma once

#include "HStandState.h"
#include "HMoveState.h"
#include "HAttackState.h"

class HNpcObject : public HObject
{
public:
	HEffect*						m_pProjectile;
	std::vector<HProjectTileInfo>	m_ProjectileList;
	HEnemyState*					m_pAction;
	std::vector<HEnemyState*>  m_pActionList;

public:
	void FSM();
	void SetTransition(DWORD dwEevnt);
public:
	virtual bool Damage()				override;
	virtual bool Dead()					override;
public:
	bool Frame() override;
	bool Render() override;
	HNpcObject()
	{
		m_iObjectType = 200;
	}
};

