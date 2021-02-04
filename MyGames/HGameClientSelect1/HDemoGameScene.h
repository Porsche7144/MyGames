#pragma once
#include "HScene.h"
#include "HNpcAirObject.h"

struct HDemoEnum : HEnum
{
	enum  HGameType1
	{
		T_AIRNPC = HEnum::T_NPC + 1,
		T_GAMEPLAYER = HEnum::T_PLAYER + 1,
	};
};
class HDemoGameScene : public HScene
{
public:
	HObject*  NewObj(int iType);

};

