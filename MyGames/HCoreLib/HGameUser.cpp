#include "HGameUser.h"
#include "HInput.h"
#include "HObjectManager.h"

bool  HGameUser::Reset()
{
	m_pProjectTileList.clear();
	return true;
}

bool HGameUser::Init()
{
	HObject::Init();
	m_pProjectTile = (HEffect*)g_ObjMgr.GetPtr(L"rtProjectile");

	return true;
}

bool HGameUser::Frame()
{
	if (g_ActionInput.bWKey)
	{
		m_ptPos.y -= m_fSpeed * (g_fSecondPerFrame+1)*0.003;
	}
	if (g_ActionInput.bAKey)
	{
		m_ptPos.x -= m_fSpeed * (g_fSecondPerFrame + 1)*0.003;
	}
	if (g_ActionInput.bDKey)
	{
		m_ptPos.x += m_fSpeed * (g_fSecondPerFrame + 1)*0.003;
	}
	if (g_ActionInput.bSKey)
	{
		m_ptPos.y += m_fSpeed * (g_fSecondPerFrame + 1)*0.003;
	}
	SetPosition(m_ptPos);

	if (m_pProjectTile && g_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
	{
		HProjectTileInfo info;
		info.bDead = false;
		info.m_fLIfeTime = 10.0f;
		info.id = 3;
		info.iMaxCount = m_pProjectTile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.pos = { m_ptPos.x, m_ptPos.y };

		m_pProjectTileList.push_back(info);
	}

	for (vector<HProjectTileInfo>::iterator iter = m_pProjectTileList.begin(); iter != m_pProjectTileList.end();)
	{
		iter->Frame();
		if (iter->bDead == true)
		{
			iter = m_pProjectTileList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}

	return true;
}

bool HGameUser::Render()
{
	HObject::Render();

	for (auto& pInfo : m_pProjectTileList)
	{
		m_pProjectTile->Set(pInfo.pos, m_pProjectTile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectTile->m_rtCollide;
		m_pProjectTile->Render();
	}

	return true;
}