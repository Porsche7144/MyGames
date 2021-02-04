#pragma once
#include "BitmapObject.h"
class HEffect : public BitmapObject
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

