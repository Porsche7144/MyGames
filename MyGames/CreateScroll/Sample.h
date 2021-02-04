#pragma once
#include "Hcore.h"
#include "HBitmap.h"

class Sample : public Hcore
{
public:
	HBitmap m_BGIntro;
	HBitmap* m_CreateDIB;
	BLENDFUNCTION m_BlendFunction;
	HBitmap* m_pDlg;

public:
	void GetRotationBitmap(float Angle, int x, int y);

public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
};

