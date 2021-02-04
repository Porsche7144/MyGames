#pragma once
#include "Hstd.h"

// 1. 게임 실행 시간 누적 기능
// 2. 1프레임 경과 시간 저장
// 3. 1초 동안 프레임 카운터


class HTimer : public HSingleton<HTimer>
{
public:
	// 1. 게임 실행 시간 누적 기능
	float	m_fGametimer;
	// 2. 1프레임 경과 시간 저장
	float	m_fSecondPerFrame;
	// 3. 1초 동안 프레임 카운터
	int		m_iFPS;

	float	m_fBeforeTime;

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

#define g_Timer  HTimer::GetInstance()

