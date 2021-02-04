#pragma once
#include "Hstd.h"

enum KetState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD
};

class HInput : public HSingleton<HInput>
{
private:
	friend class HSingleton<HInput>;
public:
	DWORD	GetKey(DWORD dKey);
	void	MsgEvent(MSG msg);
public:
	DWORD			m_dwKeyState[256];
	DWORD			m_MouseState[3];
	WCHAR			m_MouseBuffer[256];
	POINT			m_iStateValue;

public:
	// �ʱ�ȭ �� �غ� �۾�
	POINT GetPos() {return m_iStateValue;}
	bool Init();
	// ������ ������ ����۾�
	bool Frame();
	// ������ ������ ��� (��ο�, ������)
	bool Render();
	// �Ҹ� �� ���� ��� ����
	bool Release();

public:
	HInput();
	virtual ~HInput();
};

#define g_Input  HInput::GetInstance()