// TMapToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "HMapToolForm.h"


// TMapToolForm

IMPLEMENT_DYNCREATE(HMapToolForm, CFormView)
HMapToolForm* HMapToolForm::CreateOnce(CWnd* pParent)
{
	HMapToolForm* pForm = new HMapToolForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}
HMapToolForm::HMapToolForm()
	: CFormView(IDD_HMapToolForm)
	, m_TileCount(257)
	, m_CellCount("10")
	, m_Scale("10")
	, szFilename(_T("../../Image/data/main_fr_list.bmp"))
{

}

HMapToolForm::~HMapToolForm()
{
}

void HMapToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_TileCount);
	DDX_Text(pDX, IDC_EDIT2, m_CellCount);
	DDX_Text(pDX, IDC_EDIT5, m_Scale);
	DDX_Text(pDX, IDC_EDIT3, szFilename);
	DDX_Control(pDX, IDC_Texture, m_Texture);
}

BEGIN_MESSAGE_MAP(HMapToolForm, CFormView)
	ON_EN_CHANGE(IDC_EDIT1, &HMapToolForm::TileCountChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &HMapToolForm::CellCountChangeEdit)
	ON_EN_CHANGE(IDC_EDIT5, &HMapToolForm::ScaleChangeEdit)
	ON_BN_CLICKED(IDC_BUTTON2, &HMapToolForm::TextureLoadButton)
	ON_BN_CLICKED(IDOK, &HMapToolForm::OnBnClickedOk)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// TMapToolForm 진단

#ifdef _DEBUG
void HMapToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void HMapToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TMapToolForm 메시지 처리기

void HMapToolForm::SetTileCount(int TileCount)
{
	Sample::m_iTileCount = TileCount;
}

void HMapToolForm::SetCellCount(float CellCount)
{
	Sample::m_fCellCount = CellCount;
}

void HMapToolForm::SetScaleCount(float ScaleCount)
{
	Sample::m_fScale = ScaleCount;
}

void HMapToolForm::SetFileName(std::string FileName)
{
	theApp.m_Sample.m_FileName = FileName;
}

void HMapToolForm::TileCountChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_TileCount = GetDlgItemInt(IDC_EDIT1);
	SetTileCount(m_TileCount);

	if ((m_TileCount % 2) == 0)
	{
		MessageBox(L"짝수는 입력할 수 없습니다.\n", L"Error", MB_OK);
	}
}


void HMapToolForm::CellCountChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemText(IDC_EDIT2, m_CellCount);
	fCellCount = _wtof(m_CellCount.GetString());
	SetCellCount(fCellCount);
}


void HMapToolForm::ScaleChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	GetDlgItemText(IDC_EDIT5, m_Scale);
	fScale = _wtof(m_Scale.GetString());
	SetScaleCount(fScale);
}

void HMapToolForm::SetPictureControlImage(CString string)
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_Texture);
	CDC* dc = pWnd->GetDC();
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_Texture);
	CRect rect;
	staticSize->GetClientRect(rect);
	m_Img.Load(string);
	int Width = m_Img.GetWidth();
	int Height = m_Img.GetHeight();
	m_Img.Draw(dc->m_hDC, 10, 10, m_Img.GetWidth(), m_Img.GetHeight());
}

void HMapToolForm::TextureLoadButton()
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
		SetPictureControlImage(pathName);

		pathName = PathFindFileName(pathName);
		//MessageBox(pathName, L"OK", MB_OK);

		szFilename.Format(pathName);

		if (pathName)
		{
			pathName.ReleaseBuffer();
		}
		
	}


	UpdateData(FALSE);
}


void HMapToolForm::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_Sample.Init();
}


int HMapToolForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_TileCount = 257;
	SetTileCount(m_TileCount);

	fCellCount = 10.0f;
	SetCellCount(fCellCount);

	fScale = 10.0f;
	SetScaleCount(fScale);

	string filename = "../../Image/data/main_fr_list.bmp";
	//CString pathName = L"../../Image/data/main_fr_list.bmp";
	//SetPictureControlImage(pathName);
	SetFileName(filename);

	return 0;
}
