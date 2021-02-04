#pragma once
#include "Hstd.h"


class HBitmap
{
public:
	HBITMAP		m_hBitmap;
	BITMAP		m_BitmapInfo;
	HDC			m_hMemDC;
	int			m_srcSelect;
	BLENDFUNCTION m_BlendFunction;
	LPBYTE	m_pRaster;


public:
	bool Load(wstring filename);
	bool Draw(RECT rt, RECT src, DWORD op = SRCCOPY, int Select = 0);
	bool DrawTransParent(RECT desk, RECT src, DWORD color);
	HBITMAP  LoadDIB(wstring filename);
	bool  DrawAlphaBlend(RECT desk, RECT src, BYTE alpha = 255, DWORD op = SRCCOPY, int iType = 0);
	bool  DrawAlphaBlend(RECT desk, RECT src, BLENDFUNCTION bf, DWORD op = SRCCOPY, int iType = 0);

	bool  DrawAlphaBlend(LONG x, LONG y, BYTE alpha = 255, DWORD op = SRCCOPY, int iType = 0);
	bool CreateDIB(LONG iWidth, LONG iHeight);

public:
	// 초기화 및 준비 작업
	bool Init();
	// 프레임 단위로 계산작업
	bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	bool Render();
	// 소멸 및 삭제 기능 구현
	bool Release();

public:
	HBitmap();
	virtual ~HBitmap();
};

