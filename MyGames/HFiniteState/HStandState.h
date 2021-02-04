#pragma once
#include "HEnemyState.h"
class HStandState :
    public HEnemyState
{

public:
    void Process(HObject* pPlayer);
    HStandState(HObject* owner);
};

