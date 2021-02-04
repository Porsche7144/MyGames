#include "HSceneInGame.h"


bool HSceneInGame::Frame()
{
	HScene::Frame();
	for (BitmapObject* pObj : m_Object)
	{
		if (HCollision::RectToRect(pObj->m_rtCollide, m_pGamePlayer->m_rtCollide))
		{
			m_bGameOver = true;
			int check = MessageBox(g_hwnd, L"���� ����", L"���", MB_ICONQUESTION | MB_OKCANCEL);
			
			if (check == IDOK)
			{
				return WM_DESTROY;
			}
		}
	}

	return true;
}

HSceneInGame::HSceneInGame()
{
	m_bGameOver = false;
}