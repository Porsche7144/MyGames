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
	// 초기화 및 준비 작업
	bool init();
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
