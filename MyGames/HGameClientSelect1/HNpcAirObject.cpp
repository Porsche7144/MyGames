#include "HNpcAirObject.h"
#include "HScene.h"

void HNpcAirObject::Attack(HObject* pPlayer)
{
	if (m_pProjectile)
	{
		HProjectTileInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;// rand() % 4;
		info.iMaxCount = m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };
		TPoint vDir = pPlayer->m_ptPos - m_ptPos;
		info.m_vDir = TPoint(0.0f, 1.0f);
		m_ProjectileList.push_back(info);
	}
}
bool HNpcAirObject::Frame()
{
	TNpcObject::Frame();
	for (std::vector<TProjectileInfo>::iterator iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end();
		)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_ProjectileList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	for (TProjectileInfo& project : m_ProjectileList)
	{
		if (TCollision::Rect2Rect(
			project.m_rtCollide,
			TScene::m_pGamePlayer->m_rtCollide))
		{
			TScene::m_pCurrentScene->AddEffect(L"rtExplosion", project.p);
			project.m_bDead = true;
			if (TScene::m_pGamePlayer->Damage())
			{
				TScene::m_pCurrentScene->m_bSceneChange = true;
				TScene::m_iStageCounter = 0;
			}
		}
	}
	return true;
}
bool TNpcAirObject::Render()
{
	TNpcObject::Render();
	for (auto& pInfo : m_ProjectileList)
	{
		m_pProjectile->Set(pInfo.p,
			m_pProjectile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectile->m_rtCollide;
		m_pProjectile->Render();
	}
	return true;
}
bool TNpcAirObject::Damage()
{
	return false;
}
bool TNpcAirObject::Dead()
{
	return true;
}