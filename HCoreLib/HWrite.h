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
	// 초기화 및 준비 작업
	bool Init();
	// 프레임 단위로 계산작업
	bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	bool Render();
	// 소멸 및 삭제 기능 구현
	bool Release();
};

#define g_Write  HWrite::GetInstance()