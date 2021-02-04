#pragma once
#include "Hstd.h"

// 1. ���� ���� �ð� ���� ���
// 2. 1������ ��� �ð� ����
// 3. 1�� ���� ������ ī����


class HTimer : public HSingleton<HTimer>
{
public:
	// 1. ���� ���� �ð� ���� ���
	float	m_fGametimer;
	// 2. 1������ ��� �ð� ����
	float	m_fSecondPerFrame;
	// 3. 1�� ���� ������ ī����
	int		m_iFPS;

	float	m_fBeforeTime;

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

#define g_Timer  HTimer::GetInstance()

