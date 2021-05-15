// HMapModifyTool.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "HMapModifyTool.h"


// HMapModifyTool

IMPLEMENT_DYNCREATE(HMapModifyTool, CFormView)
HMapModifyTool* HMapModifyTool::CreateOnce(CWnd* pParent)
{
	HMapModifyTool* pModify = new HMapModifyTool;
	pModify->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pModify;
}

HMapModifyTool::HMapModifyTool()
	: CFormView(IDD_HMapModifyTool)
	, m_csRadius("100")
	, m_Speed(_T("1.0"))
{

}

HMapModifyTool::~HMapModifyTool()
{
}

void HMapModifyTool::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_csRadius);
	DDX_Control(pDX, IDC_BUTTON1, bIncrease);
	DDX_Text(pDX, IDC_EDIT6, m_Speed);
	
}

BEGIN_MESSAGE_MAP(HMapModifyTool, CFormView)
	ON_EN_CHANGE(IDC_EDIT2, &HMapModifyTool::RadiusChangeEdit)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON1, &HMapModifyTool::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON2, &HMapModifyTool::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &HMapModifyTool::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT6, &HMapModifyTool::SpeedChangeEdit)
	ON_BN_CLICKED(IDC_BUTTON4, &HMapModifyTool::OnBnClickedButton4)
	ON_LBN_SELCHANGE(IDC_LIST3, &HMapModifyTool::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON7, &HMapModifyTool::SplattingOnButton)
	ON_BN_CLICKED(IDC_BUTTON8, &HMapModifyTool::SplattingOffButton)
	ON_BN_CLICKED(IDC_RADIO4, &HMapModifyTool::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO5, &HMapModifyTool::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO8, &HMapModifyTool::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO9, &HMapModifyTool::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON9, &HMapModifyTool::FieldUpdateOnButton)
	ON_BN_CLICKED(IDC_BUTTON6, &HMapModifyTool::FieldUpdateOffButton)
END_MESSAGE_MAP()


// HMapModifyTool 진단

#ifdef _DEBUG
void HMapModifyTool::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void HMapModifyTool::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// HMapModifyTool 메시지 처리기

void HMapModifyTool::SetRadiusCount(float radius)
{
	theApp.m_Sample.m_fRadius = radius;
}

void HMapModifyTool::SetSpeedCount(float speed)
{
	theApp.m_Sample.m_fSpeed = speed;
}

void HMapModifyTool::RadiusChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemText(IDC_EDIT2, m_csRadius);
	m_fRadius = _wtof(m_csRadius.GetString());
	SetRadiusCount(m_fRadius);
}


int HMapModifyTool::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_fRadius = 100.0f;
	SetRadiusCount(m_fRadius);

	m_fSpeed = 1.0f;
	SetSpeedCount(m_fSpeed);
	
	

	return 0;
}


void HMapModifyTool::OnBnClickedButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bIncreaseGround = true;
	theApp.m_Sample.m_bDecreaseGround = false;
	theApp.m_Sample.m_bOriginGround = false;
	theApp.m_Sample.m_bFlatGrond = false;
}


void HMapModifyTool::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bDecreaseGround = true;
	theApp.m_Sample.m_bIncreaseGround = false;
	theApp.m_Sample.m_bOriginGround = false;
	theApp.m_Sample.m_bFlatGrond = false;
}


void HMapModifyTool::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bOriginGround = true;
	theApp.m_Sample.m_bDecreaseGround = false;
	theApp.m_Sample.m_bIncreaseGround = false;
	theApp.m_Sample.m_bFlatGrond = false;

}


void HMapModifyTool::SpeedChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemText(IDC_EDIT6, m_Speed);
	m_fSpeed = _wtof(m_Speed.GetString());
	SetSpeedCount(m_fSpeed);
}


void HMapModifyTool::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bFlatGrond = true;
	theApp.m_Sample.m_bOriginGround = false;
	theApp.m_Sample.m_bDecreaseGround = false;
	theApp.m_Sample.m_bIncreaseGround = false;
}


void HMapModifyTool::OnLbnSelchangeList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void HMapModifyTool::SplattingOnButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bSplattingState = true;
}


void HMapModifyTool::SplattingOffButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bSplattingState = false;
}


void HMapModifyTool::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_iSplattingNum = 1;
	MessageBox(L"woodfloor.bmp", L"선택한 이미지", MB_OK);
}


void HMapModifyTool::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_iSplattingNum = 2;
	MessageBox(L"seafloor.bmp", L"선택한 이미지", MB_OK);
}


void HMapModifyTool::OnBnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_iSplattingNum = 3;
	MessageBox(L"stone_wall.bmp", L"선택한 이미지", MB_OK);
}


void HMapModifyTool::OnBnClickedRadio4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_iSplattingNum = 4;
	MessageBox(L"land.bmp", L"선택한 이미지", MB_OK);
}


void HMapModifyTool::FieldUpdateOnButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bFieldUpdateState = true;
}


void HMapModifyTool::FieldUpdateOffButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.m_bFieldUpdateState = false;
}
