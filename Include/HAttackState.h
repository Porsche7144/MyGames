#pragma once
#include "HEnemyState.h"
class HAttackState :
    public HEnemyState
{

public:
    void Process(HObject* pPlayer);
    HAttackState(HObject* owner);
};

