// HMapModifyPane.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "HMapModifyPane.h"


// HMapModifyPane

IMPLEMENT_DYNAMIC(HMapModifyPane, CDockablePane)

HMapModifyPane::HMapModifyPane()
{

}

HMapModifyPane::~HMapModifyPane()
{
}


BEGIN_MESSAGE_MAP(HMapModifyPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// HMapModifyPane 메시지 처리기

int HMapModifyPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wmModify = HMapModifyTool::CreateOnce(this);

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void HMapModifyPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wmModify)
	{
		m_wmModify->SetWindowPos(NULL, 0, 0, cx, cy,
			SWP_NOZORDER);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

int HMapModifyPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
}
