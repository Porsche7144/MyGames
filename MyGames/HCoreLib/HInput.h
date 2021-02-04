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
	// 초기화 및 준비 작업
	POINT GetPos() {return m_iStateValue;}
	bool Init();
	// 프레임 단위로 계산작업
	bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	bool Render();
	// 소멸 및 삭제 기능 구현
	bool Release();

public:
	HInput();
	virtual ~HInput();
};

#define g_Input  HInput::GetInstance()