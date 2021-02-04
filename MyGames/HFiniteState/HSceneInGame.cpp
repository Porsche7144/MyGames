#include "HSceneInGame.h"

bool  HSceneInGame::CreateScene()
{

	HScene::CreateScene();

	for (HObjAttribute& info : m_ObjAttribute)
	{
		if (!(info.iObjState >= 200 &&
			info.iObjState < 300)) continue;

		for (int iObj = 0; iObj < m_iStageCounter; iObj++)
		{
			HObject* pObj = NewObj(info.iObjState);
			pObj->Init();
			if (info.bRGBKey == false)
			{
				pObj->Load(info.OriginBitmap.c_str(),
					info.MaskBitmap.c_str());
			}
			else
			{
				pObj->Load(info.OriginBitmap.c_str(),
					nullptr,
					info.dwRGB);
			}

			HPoint pos;
			pos.x = (float)(rand() % 800);
			pos.y = (float)(rand() % 300);
			pObj->Set(pos, info.rtSrc);
			m_ObjList.push_back(pObj);
		}
	}
	m_iMaxCounter = m_ObjList.size();
	return true;
}


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

	for (HObject* pObj : m_ObjList)
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