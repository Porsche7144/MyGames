// HMapObjectPane.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "HMapObjectPane.h"


// HMapObjectPane

IMPLEMENT_DYNAMIC(HMapObjectPane, CDockablePane)

HMapObjectPane::HMapObjectPane()
{

}

HMapObjectPane::~HMapObjectPane()
{
}


BEGIN_MESSAGE_MAP(HMapObjectPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// HMapObjectPane 메시지 처리기




int HMapObjectPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_pObjTool = HMapObjectModifyTool::CreateOnce(this);

	return 0;
}


void HMapObjectPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pObjTool)
	{
		m_pObjTool->SetWindowPos(NULL, 0, 0, cx, cy,
			SWP_NOZORDER);
	}
}


int HMapObjectPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	
}
