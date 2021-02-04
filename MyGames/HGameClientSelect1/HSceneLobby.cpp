#include "HSceneLobby.h"

bool HSceneLobby::Frame()
{
	HScene::Frame();
	for (auto obj : m_UIObjList)
	{
		if (obj->m_iObjectType != 301) continue;
		if (HCollision::RectPt(obj->m_rtDesk, g_Input.GetPos()))
		{
			if (g_Input.GetKey(VK_LBUTTON) == KEY_UP)
			{
				m_bSceneChange = true;
			}
		}
	}
	return true;
}