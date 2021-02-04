#pragma once
#include "Hstd.h"

struct HMsg
{
	RECT rt;
	wstring msg;
};

class HWrite : public HSingleton<HWrite>
{
public:
	vector<HMsg> m_strList;
	HFONT m_Font;
	HFONT m_oldFont;
	HDC hdc;

	void push(HMsg msg);
public:
	// �ʱ�ȭ �� �غ� �۾�
	bool Init();
	// ������ ������ ����۾�
	bool Frame();
	// ������ ������ ��� (��ο�, ������)
	bool Render();
	// �Ҹ� �� ���� ��� ����
	bool Release();
};

#define g_Write  HWrite::GetInstance()