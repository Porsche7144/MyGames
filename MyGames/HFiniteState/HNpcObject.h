#pragma once
#include "HStandState.h"
#include "HMoveState.h"
#include "HAttackState.h"

class HNpcObject : public HObject
{
public:
	HEnemyState* m_pAction;
	vector<HEnemyState*>  m_pActionList;


public:
	void FSM();
	void SetTransition(DWORD dwEvent);


	bool Frame() override;
	HNpcObject()
	{
		m_iObjectType = 200;
	}
};

