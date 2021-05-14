#pragma once



// HMapToolForm 폼 보기
#include "Sample.h"
#include "atlimage.h"

class HMapToolForm : public CFormView
{
	DECLARE_DYNCREATE(HMapToolForm)
public:
	static HMapToolForm* CreateOnce(CWnd* pParent);
protected:
	HMapToolForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~HMapToolForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HMapToolForm };
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
	int m_TileCount;
	CString m_CellCount;
	CString m_Scale;
	afx_msg void TileCountChangeEdit();
	float fCellCount;
	float fScale;

public:
	void SetTileCount(int TileCount);
	void SetCellCount(float CellCount);
	void SetScaleCount(float ScaleCount);
	void SetFileName(std::string FileName);
	void SetPictureControlImage(CString string);
	afx_msg void CellCountChangeEdit();
	afx_msg void ScaleChangeEdit();
	afx_msg void TextureLoadButton();
	afx_msg void OnBnClickedOk();
	CString szFilename;
	CStatic m_Texture;
	CImage m_Img;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


