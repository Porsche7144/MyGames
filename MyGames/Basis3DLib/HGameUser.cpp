#include "HGameUser.h"
#include "HInput.h"
#include "HObjectMgr.h"

HObject* HGameUser::Clone()
{
	return new HGameUser(*this);
}
bool HGameUser::Damage()
{
	return true;
}
bool HGameUser::Dead() 
{
	return true;
}
bool HGameUser::Hit(HPoint p, int iIndex)
{
	return true;
}

bool  HGameUser::Reset()
{
	m_ProjectileList.clear();	
	return true;
}
bool HGameUser::Init()
{
	HObject::Init();
	m_pProjectile = (HEffect*)g_ObjectMgr.GetPtr(L"rtProjectile");
	m_bAttack = false;
	m_fKeyTimer = 0.0f;
	return true;
}
bool HGameUser::ProjectileTick()
{
	if (m_pProjectile && m_bAttack)
	{
		HProjectileInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;// rand() % 4;
		info.iMaxCount = (int)m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };

		m_ProjectileList.push_back(info);
		m_bAttack = false;
	}

	/// TODO : OtherUser projectle frame
	for (std::vector<HProjectileInfo>::iterator iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end();	)
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
	return true;
}
bool HGameUser::Frame()
{		
	m_dwMove = 0;
	HPoint dir(0, 0);
	if (g_bActive)
	{
		if (g_KeyMap.m_bRight)
		{
			dir.x = 1.0f;
			m_dwMove |= 8;
		}
		if (g_KeyMap.m_bLeft)
		{
			dir.x = -1.0f;
			m_dwMove |= 4;
		}
		if (g_KeyMap.m_bFront)
		{
			dir.y = -1.0f;
			m_dwMove |= 2;
		}
		if (g_KeyMap.m_bBack)
		{
			dir.y = 1.0f;
			m_dwMove |= 1;
		}
		m_ptDirection = dir.Normalize();
		m_ptPos.x += m_ptDirection.x * m_fSpeed * g_fSecondPerFrame;
		m_ptPos.y += m_ptDirection.y * m_fSpeed * g_fSecondPerFrame;
	}
	else
	{
		m_ptDirection = dir.Normalize();
		m_dwMove = m_dwPreMove = 0;
	}
	SetPos(m_ptPos);
	ProjectileTick();
	return true;
}

bool HGameUser::Render(ID3D11DeviceContext* pd3dContext)
{
	HObject::Render(pd3dContext);
	for (auto& pInfo : m_ProjectileList)
	{
		m_pProjectile->Set(pInfo.p,	m_pProjectile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectile->m_rtCollide;
		m_pProjectile->Render(pd3dContext);
	}
	return true;
}