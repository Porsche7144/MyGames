#include "HSceneLobby.h"

bool HSceneLobby::Frame()
{
	HScene::Frame();
	if (HCollision::RectPt(m_UIObject[2]->m_rtDesk, g_Input.GetPos()))
	{
		if (g_Input.GetKey(VK_LBUTTON) == KEY_UP)
		{
			m_bSceneChange = true;
		}
	}
	return true;
}