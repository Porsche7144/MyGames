#include "HSceneInGame.h"

bool HSceneInGame::Render()
{
	HScene::Render();
	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Render();
	}
	return true;
}


bool HSceneInGame::Frame()
{
	HScene::Frame();
	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Frame();
	}

	for (HObject* pObj : m_Object)
	{
		if (HCollision::RectToRect(pObj->m_rtCollide, m_pGamePlayer->m_rtCollide))
		{
			m_bGameOver = true;
		}

		for (HProjectTileInfo& project : m_pGamePlayer->m_pProjectTileList)
		{
			if (HCollision::RectToRect(project.m_rtCollide, pObj->m_rtCollide))
			{
				if (pObj->m_bDead == false)
				{
					AddEffect(L"rtExplosion", project.pos);
				}
				pObj->m_bDead = true;
			}
		}
	}

	return true;
}

HSceneInGame::HSceneInGame()
{
	m_bGameOver = false;
}