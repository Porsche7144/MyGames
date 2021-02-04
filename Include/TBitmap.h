#pragma once
#include "TStd.h"


class TBitmap
{
public:
	HBITMAP m_hBitmap;
	HDC		m_hMemDC;
	LPBYTE	m_pRaster;
	BITMAP  m_BitmapInfo;
	BLENDFUNCTION m_BlendFunction;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
public:
	HBITMAP  LoadDIB(wstring filename);
	bool  CreateDIB(LONG iWidth, LONG iHeight, int iBitCount =24);
	bool  Load(wstring filename);

	bool  Draw(RECT desk, RECT src, DWORD op=SRCCOPY, int iType=0);
	bool  DrawColorKey(LONG x, LONG Y, DWORD color);
	bool  DrawColorKey(	RECT desk,
						RECT src, DWORD color=RGB(255,0,255));
	bool  DrawAlphaBlend(RECT desk, RECT src,BYTE alpha = 255);
	bool  DrawAlphaBlend(RECT desk, RECT src,BLENDFUNCTION bf);
	bool  DrawAlphaBlend(LONG x, LONG y, BYTE alpha = 255);
	bool  DrawAlphaBlend(LONG x, LONG y, BYTE alpha, BLENDFUNCTION bf);
};

