#include "HWrite.h"


void   HWrite::push(HMsg msg)
{
	if (m_strList.size() > 10)
	{
		m_strList.pop_back();
	}
	m_strList.insert(
		m_strList.begin(),
		msg);
}

// 초기화 및 준비 작업
bool HWrite::Init()
{
	// 폰트 초기화
	m_Font = CreateFont(25, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, _T("바탕"));
	SelectObject(g_hOffScreenDC, m_Font);
	return true;
}
// 프레임 단위로 계산작업
bool HWrite::Frame()
{
	return true;
}
// 프레임 단위로 출력 (드로우, 렌더링)
bool HWrite::Render()
{
		//// 폰트설정
		//m_Font = CreateFont(20, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, _T("바탕"));
		//SelectObject(g_hOffScreenDC, m_Font);
		SetTextColor(g_hOffScreenDC, RGB(255, 0, 0)); // 텍스트 컬러
		SetBkColor(g_hOffScreenDC, RGB(0, 0, 0)); // 텍스트 배경 컬러
		// SetBkMode(g_hOffScreenDC, TRANSPARENT); // 투명
	return true;

}
// 소멸 및 삭제 기능 구현
bool HWrite::Release()
{
	ReleaseDC(g_hwnd, hdc);

	SelectObject(hdc, m_oldFont);
	DeleteObject(m_Font);
	return true;
}