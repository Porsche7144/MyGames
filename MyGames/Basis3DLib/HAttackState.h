#pragma once
#include "HProjectile.h"
#include "HEnemyState.h"
class HAttackState :public HEnemyState
{
public:
	void Process(HObject* pPlayer);
public:
	HAttackState(HObject* owner);
};

