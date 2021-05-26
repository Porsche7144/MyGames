#pragma once


// HCharactorToolDlg 대화 상자

class HCharactorToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(HCharactorToolDlg)

public:
	HCharactorToolDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~HCharactorToolDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HCharactorToolDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
