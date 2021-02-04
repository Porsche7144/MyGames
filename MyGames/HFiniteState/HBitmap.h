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
	// �ʱ�ȭ �� �غ� �۾�
	bool Init();
	// ������ ������ ����۾�
	bool Frame();
	// ������ ������ ��� (��ο�, ������)
	bool Render();
	// �Ҹ� �� ���� ��� ����
	bool Release();

public:
	HBitmap();
	virtual ~HBitmap();
};

