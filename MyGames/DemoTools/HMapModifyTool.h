#pragma once



// HMapModifyTool 폼 보기

class HMapModifyTool : public CFormView
{
	DECLARE_DYNCREATE(HMapModifyTool)
public:
	static HMapModifyTool* CreateOnce(CWnd* pParent);
protected:
	HMapModifyTool();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~HMapModifyTool();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HMapModifyTool };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void SetRadiusCount(float radius);
	

	DECLARE_MESSAGE_MAP()
public:
	// // 반지름 범위
	CString m_csRadius;
	float m_fRadius;
	afx_msg void RadiusChangeEdit();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// // 지형 올리기
	CButton bIncrease;
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	// // 속력
	CString m_Speed;
	float m_fSpeed;
	afx_msg void SpeedChangeEdit();
	void SetSpeedCount(float speed);
	afx_msg void OnBnClickedButton4();
};


