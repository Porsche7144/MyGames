// HCharactorToolDlg.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "HCharactorToolDlg.h"
#include "afxdialogex.h"


// HCharactorToolDlg 대화 상자

IMPLEMENT_DYNAMIC(HCharactorToolDlg, CDialogEx)

HCharactorToolDlg::HCharactorToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HCharactorToolDlg, pParent)
{

}

HCharactorToolDlg::~HCharactorToolDlg()
{
}

void HCharactorToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HCharactorToolDlg, CDialogEx)
END_MESSAGE_MAP()


// HCharactorToolDlg 메시지 처리기
