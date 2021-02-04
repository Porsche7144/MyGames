#pragma once
#include "TStd.h"
// 1, ���� ���� �ð� ���� ���
// 2, 1 Frame ��� �ð� ����
// 3, 1�� ���� ������ ī����
// ********* ��� Ŭ������ �ݵ�� ������ ���� �Լ��� �����Ѵ�. *******

class TTimer : public TSingleton<TTimer>
{
public:
	// 1, ���� ���� �ð� ���� ���
	float	m_fGameTimer;
	// 2, 1 Frame ��� �ð� ����
	float   m_fSecondPerFrame;
	// 3, 1�� ���� ������ ī����
	int		m_iFPS;

	WCHAR	m_szBuffer[256] = { 0, };
	float   m_fBeforeTime;
public:
	bool  Reset();
	// �ʱ�ȭ �� �غ� �۾� ����
	bool  Init();
	// ������ ������ ����۾�
	bool  Frame();
	// ������ ������ ��ο�(������)
	bool  Render();
	// �Ҹ� �� ���� ��� ����
	bool  Release();
public:
	TTimer();
	virtual ~TTimer();
};
#define g_Timer TTimer::GetInstance()
