﻿
// SampleMFCView.h: CSampleMFCView 클래스의 인터페이스
//

#pragma once


class CSampleMFCView : public CView
{
protected: // serialization에서만 만들어집니다.
	CSampleMFCView() noexcept;
	DECLARE_DYNCREATE(CSampleMFCView)

// 특성입니다.
public:
	CSampleMFCDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CSampleMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SampleMFCView.cpp의 디버그 버전
inline CSampleMFCDoc* CSampleMFCView::GetDocument() const
   { return reinterpret_cast<CSampleMFCDoc*>(m_pDocument); }
#endif

