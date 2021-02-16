#pragma once
#include "HObject.h"

class HEffect : public HObject
{
public:
	std::vector<RECT> m_rtList;
	int		m_iRectIndex;
	float	m_fTimer;
	float	m_fStep;
public:
	void   SetSprite(std::vector<RECT>& list);
	virtual HObject* Clone() override;
	bool	Init();
	bool	Frame();
public:
	HEffect();
	virtual ~HEffect();
};

