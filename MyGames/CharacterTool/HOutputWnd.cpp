// TOutputWnd.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "HOutputWnd.h"

BEGIN_MESSAGE_MAP(HOutputList, CListBox)
END_MESSAGE_MAP()
// HOutputWnd

IMPLEMENT_DYNAMIC(HOutputWnd, CDockablePane)

HOutputWnd::HOutputWnd()
{

}

HOutputWnd::~HOutputWnd()
{
}

int HOutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 탭 창을 만듭니다.
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("출력 탭 창 생성 실패.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 출력 창을 만듭니다.
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("출력 창 생성 실패.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndTabs.AddTab(&m_wndOutputBuild, L"Build", 0);
	m_wndTabs.AddTab(&m_wndOutputDebug, L"Debug", 1);
	m_wndTabs.AddTab(&m_wndOutputFind, L"Find", 2);

	PushBuild(L"123745");
	PushBuild(L"452312345");
	PushBuild(L"45312345");
	PushBuild(L"777777");
	PushDebug(L"2222222");
	PushFind(L"555555");
	return TRUE;
}
void HOutputWnd::PushBuild(std::wstring data)
{
	m_wndOutputBuild.AddString(data.c_str());
}
void HOutputWnd::PushDebug(std::wstring data)
{
	m_wndOutputDebug.AddString(data.c_str());
}
void HOutputWnd::PushFind(std::wstring data)
{
	m_wndOutputFind.AddString(data.c_str());
}
void HOutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	m_wndTabs.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
BEGIN_MESSAGE_MAP(HOutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// HOutputWnd 메시지 처리기


