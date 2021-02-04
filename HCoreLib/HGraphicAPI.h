#pragma once
#include "Hstd.h"

class HGraphicAPI
{
public:
	HBITMAP m_hOffScreenBit;
	HBRUSH m_hbrColor;

public:
	// 초기화 및 준비 작업
	bool Init();
	// 프레임 단위로 계산작업
	bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	bool PreRender();
	bool Render();
	bool PostRender();
	// 소멸 및 삭제 기능 구현
	bool Release();
};

