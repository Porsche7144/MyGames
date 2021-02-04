#include "HDemoGameScene.h"
#include "HGamePlayer.h"

HObject*  HDemoGameScene::NewObj(int iType)
{
	HObject* pObj = HScene::NewObj(iType);
	if (pObj == nullptr)
	{
		if (iType == HDemoEnum::T_AIRNPC)
		{
			pObj = new HNpcAirObject;
			HNpcAirObject* pNpc = dynamic_cast<HNpcAirObject*>(pObj);
			if (pNpc != nullptr)
			{
				pNpc->FSM();
			}
		}
		if (iType == HDemoEnum::T_GAMEPLAYER)
		{
			pObj = new HGamePlayer;
		}
	}
	return pObj;
}