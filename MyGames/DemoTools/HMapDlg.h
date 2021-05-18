#pragma once


// HMapDlg 대화 상자
#include "Sample.h"

class HMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(HMapDlg)

public:
	HMapDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~HMapDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HMapDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	Sample m_Sample;
	int m_iTileCount;
	float m_fCellCount;
	int m_fScale;
	int m_Pos;
	// 공간분할 선택
	CComboBox m_SpaceDivision;
	afx_msg void OnCbnSelchangeCombo1();
	// 텍스쳐 선택
	afx_msg void OnLbnSelchangeList1();
	// 선택된 택스쳐 이름
	CString m_szTextureName;
	virtual BOOL OnInitDialog();
	CProgressCtrl m_Progress;
	// afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	CSpinButtonCtrl m_SpinControl;
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_ProgressCount;
	CEdit m_TileCount;

public:
	// int GetTileCount();
	void SetTileCount(int TileCount);
	void SetCellCount(float CerllCount);
	void SetScaleCount(float ScaleCount);
	void SetFileName(std::string FileName);
	afx_msg void OnBnClickedOk();
	afx_msg void CellChangeEdit();	
	afx_msg void ScaleChangeEdit();
	afx_msg void OnFileLoadButton();
};
