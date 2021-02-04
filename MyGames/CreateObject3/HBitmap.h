#pragma once
#include "Hstd.h"


class HBitmap
{
public:
	HBITMAP		m_hBitmap;
	BITMAP		m_BitmapInfo;
	HDC			m_hMemDC;
	int			m_srcSelect;


public:
	bool Load(wstring filename);
	bool Draw(RECT rt, RECT src, DWORD op = SRCCOPY, int Select = 0);
	bool DrawTransParent(RECT desk, RECT src, DWORD color);

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

