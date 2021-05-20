// HMapObjectModifyTool.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "HMapObjectModifyTool.h"


// HMapObjectModifyTool

IMPLEMENT_DYNCREATE(HMapObjectModifyTool, CFormView)
HMapObjectModifyTool* HMapObjectModifyTool::CreateOnce(CWnd* pParent)
{
	HMapObjectModifyTool* pModify = new HMapObjectModifyTool;
	pModify->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pModify;
}

HMapObjectModifyTool::HMapObjectModifyTool()
	: CFormView(IDD_HMapObjectModifyTool)
	, m_ScaleCount(_T("1.0"))
	, m_RotationCount(_T("1.0"))
{

}

HMapObjectModifyTool::~HMapObjectModifyTool()
{
}

void HMapObjectModifyTool::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ObjName);
	DDX_Text(pDX, IDC_EDIT2, m_ScaleCount);
	DDX_Text(pDX, IDC_EDIT6, m_RotationCount);
}

BEGIN_MESSAGE_MAP(HMapObjectModifyTool, CFormView)
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_LIST1, &HMapObjectModifyTool::OnMapObjSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON1, &HMapObjectModifyTool::ObjDeleteClickedButton)
	ON_BN_CLICKED(IDC_BUTTON4, &HMapObjectModifyTool::AddObjClickedButton)
	ON_BN_CLICKED(IDC_BUTTON9, &HMapObjectModifyTool::ObjMoveClickedButton)
	ON_EN_CHANGE(IDC_EDIT2, &HMapObjectModifyTool::OnScaleCountChangeEdit)
	ON_EN_CHANGE(IDC_EDIT6, &HMapObjectModifyTool::OnRotationCountChangeEdit)
END_MESSAGE_MAP()


// HMapObjectModifyTool 진단

#ifdef _DEBUG
void HMapObjectModifyTool::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void HMapObjectModifyTool::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// HMapObjectModifyTool 메시지 처리기


int HMapObjectModifyTool::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void HMapObjectModifyTool::OnMapObjSelchangeList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString str;
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST1);
	int index = pList->GetCurSel();
	if (index != LB_ERR)
	{
		pList->GetText(index, str);
	}
	
	CString str2 = L"../../Image/FBX_Image/";
	str = str2 + str;
	theApp.m_Sample.m_pObj.m_ObjName = CW2A(str);
	theApp.m_Sample.m_pObj.Init();
	//theApp.m_Sample.m_bObjInitState = true;
	theApp.m_Sample.m_bObjFrameState = true;

}


void HMapObjectModifyTool::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_ObjName.InsertString(0, L"rockBlobOne.fbx");
	m_ObjName.SetCurSel(0);

}


void HMapObjectModifyTool::ObjDeleteClickedButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bObjDelete = true;
	theApp.m_Sample.m_bCreateObj = false;
	theApp.m_Sample.m_bMoveObj = false;
	theApp.m_Sample.m_bScale = false;
	theApp.m_Sample.m_bRotation = false;
}


void HMapObjectModifyTool::AddObjClickedButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bCreateObj = true;
	theApp.m_Sample.m_bObjDelete = false;
	theApp.m_Sample.m_bMoveObj = false;
	theApp.m_Sample.m_bScale = false;
	theApp.m_Sample.m_bRotation = false;

}


void HMapObjectModifyTool::ObjMoveClickedButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bMoveObj = true;
	theApp.m_Sample.m_bCreateObj = false;
	theApp.m_Sample.m_bObjDelete = false;
	theApp.m_Sample.m_bScale = false;
	theApp.m_Sample.m_bRotation = false;
}


void HMapObjectModifyTool::OnScaleCountChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemText(IDC_EDIT2, m_ScaleCount);
	float fScaleCount = _wtof(m_ScaleCount.GetString());
	theApp.m_Sample.m_ScaleCount = fScaleCount;
	theApp.m_Sample.m_bScale = true;
	theApp.m_Sample.m_bRotation = false;
	theApp.m_Sample.m_bMoveObj = false;
	theApp.m_Sample.m_bCreateObj = false;
	theApp.m_Sample.m_bObjDelete = false;

}


void HMapObjectModifyTool::OnRotationCountChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemText(IDC_EDIT6, m_RotationCount);
	float fRotationCount = _wtof(m_RotationCount.GetString());
	theApp.m_Sample.m_RotationCount = fRotationCount;
	theApp.m_Sample.m_bRotation = true;
	theApp.m_Sample.m_bScale = false;
	theApp.m_Sample.m_bMoveObj = false;
	theApp.m_Sample.m_bCreateObj = false;
	theApp.m_Sample.m_bObjDelete = false;
}
