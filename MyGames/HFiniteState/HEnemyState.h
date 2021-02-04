#pragma once
#include "HObject.h"
#include "HFiniteState.h"


class HEnemyState
{

public:
	HObject*	m_pOwner;
	DWORD			m_dwState;

public:
	virtual void Process(HObject* pPlayer) = 0;
	HEnemyState(HObject* owner);
};

