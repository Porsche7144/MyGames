#pragma once
#include "Hstd.h"

class HGraphicAPI
{
public:
	HBITMAP m_hOffScreenBit;
	HBRUSH m_hbrColor;

public:
	// �ʱ�ȭ �� �غ� �۾�
	bool Init();
	// ������ ������ ����۾�
	bool Frame();
	// ������ ������ ��� (��ο�, ������)
	bool PreRender();
	bool Render();
	bool PostRender();
	// �Ҹ� �� ���� ��� ����
	bool Release();
};

