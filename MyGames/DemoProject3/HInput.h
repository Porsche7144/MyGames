#pragma once
#include "Hstd.h"

enum KetState
{
	KEY_FREE = 0,
	KEY_PUSH,
	KEY_HOLD,
	KEY_UP
};


class HInput : public HSingleton<HInput>
{
private:
	friend class HSingleton<HInput>;
public:
	DWORD GetKey(DWORD dKey);
public:
	vector<wstring> m_strList;
	TCHAR m_csBuffer[256];
	DWORD m_dwKeyState[256];
	DWORD m_MouseState[3];
	POINT m_MousePos;
public:
	// �ʱ�ȭ �� �غ� �۾�
	bool init();
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
