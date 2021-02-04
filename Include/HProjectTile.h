#pragma once
#include "HEffect.h"

typedef vector<RECT> RECT_ARRAY;

struct HEffectInfo
{
	wstring		m_csName;
	HPoint		pos;
	int			id;
	int			iMaxCount;
	bool		bDead;
	float		m_fLIfeTime;
	float		 m_fTimer;
	float		m_fStep;
	int			m_iRectIndex;
	RECT		m_rtSrc;
	RECT		m_rtDesk;

	void Frame()
	{
		m_fLIfeTime -= (g_fSecondPerFrame + 2)*0.001;
		if (m_fLIfeTime <= 0.0f)
		{
			bDead = true;
			return;
		}
		m_fTimer += (g_fSecondPerFrame + 2)*0.001;
		if (m_fTimer >= m_fStep)
		{
			m_iRectIndex++;
			if (iMaxCount <= m_iRectIndex)
			{
				m_iRectIndex = 0.0f;
			}
			m_fTimer -= m_fStep;
		}
	}

	HEffectInfo()
	{
		bDead = false;
		m_fLIfeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 1.0f;
		m_iRectIndex = 0;
	}
};

struct HProjectTileInfo
{
	wstring		m_csName;
	HPoint		pos;
	int			id;
	int			iMaxCount;
	bool		bDead;
	float		m_fLIfeTime;
	float		 m_fTimer;
	float		m_fStep;
	int			m_iRectIndex;
	RECT		m_rtSrc;
	RECT		m_rtDesk;
	RECT		m_rtCollide;

	void Frame()
	{
		m_fLIfeTime -= (g_fSecondPerFrame + 2)*0.001;
		if (m_fLIfeTime <= 0.0f)
		{
			bDead = true;
			return;
		}
		m_fTimer += (g_fSecondPerFrame + 2)*0.001;
		if (m_fTimer >= m_fStep)
		{
			m_iRectIndex++;
			if (iMaxCount <= m_iRectIndex)
			{
				m_iRectIndex = 0;
			}
			m_fTimer -= m_fStep;
		}

		pos = pos + (HPoint(0.0f, -1.0f) * g_fSecondPerFrame * 300.0f);
	}

	HProjectTileInfo()
	{
		bDead = false;
		m_fLIfeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 1.0f;
		m_iRectIndex = 0;
	}
};

class HProjectTile : public HEffect
{
public:
};

