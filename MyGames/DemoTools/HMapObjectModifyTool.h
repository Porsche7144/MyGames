#pragma once



// HMapObjectModifyTool 폼 보기

class HMapObjectModifyTool : public CFormView
{
	DECLARE_DYNCREATE(HMapObjectModifyTool)

public:
	static HMapObjectModifyTool* CreateOnce(CWnd* pParent);

protected:	
	HMapObjectModifyTool();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~HMapObjectModifyTool();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HMapObjectModifyTool };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// // 오브젝트 이름
	CListBox m_ObjName;
	afx_msg void OnMapObjSelchangeList();
	virtual void OnInitialUpdate();
	afx_msg void ObjDeleteClickedButton();
	afx_msg void AddObjClickedButton();
};


