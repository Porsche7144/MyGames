#pragma once
#include "Hcore.h"
#include "HBitmap.h"

class Sample : public Hcore
{
	HBitmap m_BGIntro;
	HBitmap* m_CreateDIB;
	BLENDFUNCTION m_BlendFunction;
	HBitmap* m_pDlg;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
};

