// HPropertiesWnd.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "HPropertiesWnd.h"


// HPropertiesWnd

IMPLEMENT_DYNAMIC(HPropertiesWnd, CDockablePane)

HPropertiesWnd::HPropertiesWnd()
{

}

HPropertiesWnd::~HPropertiesWnd()
{
}


BEGIN_MESSAGE_MAP(HPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()



// TPropertiesWnd 메시지 처리기




int HPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	//CRect rt;
	//rt.SetRectEmpty();
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD,
		CRect(0, 0, 100, 100), this, 3333))
	{
		return -1;
	}
	InitPropList();
	return 0;
}

void HPropertiesWnd::InitPropList()
{
	// 그룹
	CMFCPropertyGridProperty* pGroup1 = nullptr;
	pGroup1 = new CMFCPropertyGridProperty(L"111");
	pGroup1->SetData(11111);
	CMFCPropertyGridProperty* pGroup2 = nullptr;
	pGroup2 = new CMFCPropertyGridProperty(L"222");
	pGroup1->SetData(22222);

	CMFCPropertyGridProperty* pData =
		new CMFCPropertyGridProperty(L"a1", L"Dlg");
	pData->AddOption(L"00001");
	pData->AddOption(L"00002");
	pData->AddOption(L"00003");
	pData->AddOption(L"00004");
	pData->AllowEdit(FALSE);
	pData->SetData(11000);
	pGroup1->AddSubItem(pData);

	CMFCPropertyGridProperty* pSize =
		new CMFCPropertyGridProperty(L"크기", 50l,
			L"크기를 지정한다.");
	pSize->EnableSpinControl(TRUE, 0, 100);
	pSize->SetData(11100);
	pGroup1->AddSubItem(pSize);

	CMFCPropertyGridColorProperty* pColor =
		new CMFCPropertyGridColorProperty(L"컬러", RGB(128, 128, 128),
			nullptr,
			L"크기를 지정한다.");
	pColor->EnableOtherButton(L"기타");
	pColor->EnableAutomaticButton(L"기본값", ::GetSysColor(COLOR_3DFACE));
	pColor->SetData(11110);
	pGroup1->AddSubItem(pColor);

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("항목 1"), (_variant_t)_T("값 1"), _T("설명입니다."), 9100));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("항목 2"), (_variant_t)_T("값 2"), _T("설명입니다."), 9101));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("항목 3"), (_variant_t)_T("값 3"), _T("설명입니다."), 9102));

	pGroup1->AddSubItem(new CMFCPropertyGridFileProperty(_T("폴더"), _T("c:\\"), 9200));

	m_wndPropList.AddProperty(pGroup1);
	m_wndPropList.AddProperty(pGroup2);
}
void HPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	CRect rt;
	GetClientRect(rt);
	m_wndPropList.SetWindowPos(NULL, rt.left, rt.top,
		rt.right, rt.bottom,
		SWP_NOZORDER | SWP_NOACTIVATE);
}



void HPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
LRESULT HPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	COleVariant rVariant = pProp->GetValue();
	VARIANT var = rVariant.Detach();
	switch ((int)pProp->GetData())
	{
		//case idShowMDITabs:
		//{
		//	CString strShowMdiTabOption = (LPCTSTR)(_bstr_t)pProp->GetValue();
		//	m_Data.bArray[0] = pProp->GetValue().boolVal == VARIANT_TRUE;
		//}
		//break;

		//case idTabsOnTop:
		//{
		//	CString strStyle = (LPCTSTR)(_bstr_t)pProp->GetValue();
		//	m_Data.szArray[0] = strStyle;
		//}break;
		//break;
		//case idScreenSize:
		//{
		//	for (int i = 0; i < pProp->GetSubItemsCount(); i++)
		//	{
		//		CMFCPropertyGridProperty* pSubProp = pProp->GetSubItem(i);
		//		int iValue = (int)pSubProp->GetValue().intVal;
		//		m_Data.iArray[i] = iValue;
		//	}
		//}break;
		//case idHeight:
		//{
		//	int iValue = (int)pProp->GetValue().intVal;
		//	m_Data.iArray[0] = iValue;
		//}
		//break;
		//case idTabsStyle:
		//{
		//	CMFCPropertyGridFontProperty* pFont = (CMFCPropertyGridFontProperty*)pProp;
		//	CString strStyle = pFont->FormatProperty();
		//	LPLOGFONT logFont = pFont->GetLogFont();
		//	COLORREF color = pFont->GetColor();
		//	m_Data.szArray[3] = strStyle;
		//}
		//break;

		//case idTabsAutoColor:
		//{
		//	CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*)pProp;
		//	COLORREF color = pColor->GetColor();
		//	break;
		//}break;

		//case idMDITabsIcons:
		//{
		//	CMFCPropertyGridFileProperty* File = (CMFCPropertyGridFileProperty*)pProp;
		//	CString strStyle = (LPCTSTR)(_bstr_t)File->GetValue();
		//	m_Data.szArray[4] = strStyle;
		//}break;
	}
	return 0;
}
