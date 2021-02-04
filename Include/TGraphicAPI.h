#pragma once
#include "TStd.h"
class TGraphicAPI
{
public:
	HDC		m_hScreenDC;
	HDC		m_hOffScreenDC;
	HBITMAP m_hOffScreenBitmap;
	HBRUSH  m_hbrRed;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	PreRender();
	bool	PostRender();
	bool	Release();
};

