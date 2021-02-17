#pragma once
#include "HObject.h"
#include "HFiniteState.h"
class HEnemyState
{
public:
	HObject*    m_pOwner;
	DWORD		m_dwState;
	float		m_fTimer;
	float		m_fTransitionInterval;
public:
	virtual void Process(HObject* pPlayer) = 0;
	HEnemyState(HObject* owner);
};

