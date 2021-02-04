#pragma once
#include "HEnemyState.h"
class HMoveState :
    public HEnemyState
{

public:
    void Process(HObject* pPlayer);
    HMoveState(HObject* owner);
};

