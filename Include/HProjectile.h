#pragma once
#include "HEffect.h"

struct HEffectInfo
{
	wstring     m_csName;
	HPoint		p;
	HPoint		d;
	int			id;
	int			iMaxCount;
	bool		m_bDead;
	float		m_fLifeTime;
	float		m_fTimer;
	float		m_fStep;
	int			m_iRectIndex;
	RECT		m_rtSrc;
	RECT		m_rtDesk;
	void		Frame()
	{
		m_fLifeTime -= g_fSecondPerFrame;
		if (m_fLifeTime <= 0.0f)
		{
			m_bDead = true;
			return;
		}
		m_fTimer += g_fSecondPerFrame;
		if (m_fTimer >= m_fStep)
		{
			m_iRectIndex++;
			if (iMaxCount <= m_iRectIndex)
			{
				m_iRectIndex = 0;
			}
			m_fTimer -= m_fStep;
		}
		//p = p + (TPoint(0.0f, -1.0f) * g_fSecondPerFrame * 300.0f);

	}
	HEffectInfo()
	{
		m_bDead = false;
		m_fLifeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 1.0f;
		m_iRectIndex = 0;
	}
};
struct HProjectileInfo
{
	static int  g_iCount;
	int		    m_iIndex;
	wstring     m_csName;
	HPoint		p;
	HPoint      m_vDir;
	int			id;
	int			iMaxCount;
	bool		m_bDead;
	float		m_fLifeTime;
	float		m_fTimer;
	float		m_fStep;
	int			m_iRectIndex;
	RECT		m_rtSrc;
	RECT		m_rtDesk;
	RECT		m_rtCollide;
	void		Frame()
	{
		m_fLifeTime -= g_fSecondPerFrame;
		if (m_fLifeTime <= 0.0f)
		{
			m_bDead = true;
			return;
		}
		m_fTimer += g_fSecondPerFrame;
		if (m_fTimer >= m_fStep)
		{
			m_iRectIndex++;
			if (iMaxCount <= m_iRectIndex)
			{
				m_iRectIndex = 0.0f;
			}
			m_fTimer -= m_fStep;
		}
		p = p + (m_vDir * g_fSecondPerFrame * 300.0f);

	}
	HProjectileInfo()
	{
		m_vDir = HPoint(0.0f, -1.0f);
		m_bDead = false;
		m_fLifeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 1.0f;
		m_iRectIndex = 0;
	}
};

class HProjectile : public HEffect
{
	public:
};

