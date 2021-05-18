// HMapDlg.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "HMapDlg.h"
#include "afxdialogex.h"


// HMapDlg 대화 상자

IMPLEMENT_DYNAMIC(HMapDlg, CDialogEx)

HMapDlg::HMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HMapDlg, pParent)
	, m_iTileCount(0)
	, m_fCellCount(0)
	, m_szTextureName(_T(""))
	, m_ProgressCount(_T(""))
	, m_fScale(0)
{

}

HMapDlg::~HMapDlg()
{
}

void HMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iTileCount);
	DDV_MinMaxInt(pDX, m_iTileCount, 0, 4096);
	DDX_Text(pDX, IDC_EDIT2, m_fCellCount);
	DDV_MinMaxInt(pDX, m_fCellCount, 0, 4096);
	DDX_Control(pDX, IDC_COMBO1, m_SpaceDivision);
	DDX_Text(pDX, IDC_EDIT3, m_szTextureName);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_SPIN2, m_SpinControl);
	DDX_Text(pDX, IDC_EDIT4, m_ProgressCount);
	DDX_Text(pDX, IDC_EDIT5, m_fScale);
}


BEGIN_MESSAGE_MAP(HMapDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &HMapDlg::OnEnChangeEdit1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &HMapDlg::OnCbnSelchangeCombo1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &HMapDlg::OnDeltaposSpin2)
	ON_BN_CLICKED(IDOK, &HMapDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT2, &HMapDlg::CellChangeEdit)
	ON_EN_CHANGE(IDC_EDIT5, &HMapDlg::ScaleChangeEdit)
	ON_BN_CLICKED(IDC_BUTTON2, &HMapDlg::OnFileLoadButton)
END_MESSAGE_MAP()


// HMapDlg 메시지 처리기

void HMapDlg::SetTileCount(int TileCount)
{
	Sample::m_iTileCount = TileCount;
}

void HMapDlg::SetCellCount(float CellCount)
{
	Sample::m_fCellCount = CellCount;
}

void HMapDlg::SetScaleCount(float ScaleCount)
{
	Sample::m_fScale = ScaleCount;
}

void HMapDlg::SetFileName(std::string FileName)
{
	theApp.m_Sample.m_FileName = FileName;
}

void HMapDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iTileCount = GetDlgItemInt(IDC_EDIT1);
	SetTileCount(m_iTileCount);

	if ((m_iTileCount % 2) == 0)
	{
		MessageBox(L"짝수는 입력할 수 없습니다.\n", L"Error", MB_OK);
	}

}

void HMapDlg::CellChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_fCellCount = GetDlgItemInt(IDC_EDIT2);
	SetCellCount(m_fCellCount);
}

void HMapDlg::ScaleChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_fScale = GetDlgItemInt(IDC_EDIT5);
	SetScaleCount(m_fScale);
}

void HMapDlg::OnCbnSelchangeCombo1()
{
	CString name;
	int iSel = m_SpaceDivision.GetCurSel();
	m_SpaceDivision.GetLBText(iSel, name);
	UpdateData(TRUE);
}


BOOL HMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.m_Sample.m_iTileCount = 257;
	m_fCellCount = 10;
	m_fScale = 10.0f;
	string filename = "../../Image/data/main_fr_list.bmp";
	SetFileName(filename);


	SetTileCount(theApp.m_Sample.m_iTileCount);
	SetCellCount(theApp.m_Sample.m_fCellCount);
	SetScaleCount(theApp.m_Sample.m_fScale);

	m_Pos = 50;
	m_ProgressCount = L"50";

	m_SpinControl.SetRange(0, 100);
	m_SpinControl.SetPos(m_Pos);
	m_Progress.SetRange(0, 100);
	m_Progress.SetPos(50);

	// AddString과 InsetString의 차이는 정렬이 되냐 안되냐의 차이.
	m_SpaceDivision.AddString(L"QuadTree");
	m_SpaceDivision.AddString(L"OcTree");
	m_SpaceDivision.AddString(L"BSP");
	m_SpaceDivision.SetCurSel(1);

	//m_TextureList.InsertString(0, L"이미지1");
	//m_TextureList.InsertString(1, L"이미지2");
	//m_TextureList.InsertString(2, L"이미지3");
	//m_TextureList.SetCurSel(0);


	UpdateData(FALSE);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//void HMapDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	int m_Pos = m_SpinControl.GetPos();
//	m_Progress.SetPos(m_Pos);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}


void HMapDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if (pNMUpDown->iDelta < 0)
	{
		float num;
		num = pNMUpDown->iDelta / 2;
		m_Pos = pNMUpDown->iPos + num;
	}
	else
	{
		float num;
		num = pNMUpDown->iDelta / 2;
		m_Pos = pNMUpDown->iPos + num;
	}
	

	if (m_Pos >= 0 && m_Pos <= 100)
	{
		CString sValue;
		sValue.Format(L"%d", m_Pos);

		m_SpinControl.SetPos(m_Pos);
		m_Progress.SetPos(m_Pos);
		m_ProgressCount.SetString(sValue);

	}

	UpdateData(FALSE);
	*pResult = 0;
}



void HMapDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
	theApp.m_Sample.Init();
}



void HMapDlg::OnFileLoadButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG, *.DDS) | *.BMP;*.GIF;*.JPG;*.DDS;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();

		/*TCHAR szFileName[MAX_PATH];

		memset(szFileName, 0x00, sizeof(TCHAR) * MAX_PATH);

		_tcscpy_s(szFileName, MAX_PATH, pathName.GetBuffer(0));*/

		std::string filename = CW2A(pathName);

		SetFileName(filename);

		if (pathName)
		{
			pathName.ReleaseBuffer();
		}		
	}	
}
