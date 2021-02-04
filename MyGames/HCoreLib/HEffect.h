#pragma once
#include "HObject.h"
class HEffect : public HObject
{
public:
	int m_fTimer;
	int m_fStep;
	int m_iRectIndex;
	vector<RECT> m_rtList;
	void SetSprite(vector<RECT>& list);

public:
	bool Frame();


public:
	HEffect();
	virtual ~HEffect();


};

