#pragma once
#include "Hcore.h"
#include "HEffect.h"


struct HEffectInfo
{
	POINT pos;
	int id;
	int iMaxCount;
	bool bDead;
	float m_fLIfeTime;
	float m_fTimer;
	float m_fStep;
	int m_iRectIndex;
	RECT m_rtSrc;
	RECT m_rtDesk;

	void Frame()
	{
		m_fLIfeTime -= (g_fSecondPerFrame+2)*0.001;
		if (m_fLIfeTime <= 0.0f)
		{
			bDead = true;
		}
		m_fTimer += (g_fSecondPerFrame +2)*0.001;
		if (m_fTimer >= m_fStep)
		{
			m_iRectIndex++;
			m_fTimer -= m_fStep;
		}
	}

	HEffectInfo()
	{
		bDead = false;
		m_fLIfeTime = 1.0f;
		m_fTimer = 0.0f;
		m_fStep = 0.0f;
		m_iRectIndex = 0;
	}
};

typedef vector<RECT> RECT_ARRAY;

class Sample : public Hcore
{
public:
	HPoint		m_BitObPos;

	vector<HEffect*>		m_EffectList;
	vector<RECT_ARRAY>		m_rtSpriteList;
	vector<HEffectInfo>		m_EffectInfoList;

public:
	bool GameDataLoad(const TCHAR* pszLoad);
public:
	// 초기화 및 준비 작업
	bool Init();
	// 프레임 단위로 계산작업
	bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	bool Render();
	// 소멸 및 삭제 기능 구현
	bool Release();
};

