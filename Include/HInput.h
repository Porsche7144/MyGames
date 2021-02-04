#pragma once
#include "HStd.h"
enum KeyState
{
	KEY_FREE = 0,
	KEY_UP,

	KEY_PUSH,	
	KEY_HOLD
};
class HInput : public HSingleton<HInput>
{
public:
	friend  class HSingleton<HInput>;
private:
	DWORD	m_dwKeyState[256];
	POINT   m_MousePos;	
	bool	m_bEnable;
public:
	void	SetEnable(bool bEnable = true);
	bool	GetEnable();
	POINT   GetPos() { return m_MousePos; }
	bool	Reset();
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	DWORD GetKey(DWORD dwKey);
private:
	HInput();
public:
	~HInput();
};
#define g_Input HInput::GetInstance()
