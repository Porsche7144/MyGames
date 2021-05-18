#pragma once


// HMapObjectPane
#include "HMapObjectModifyTool.h"

class HMapObjectPane : public CDockablePane
{
	DECLARE_DYNAMIC(HMapObjectPane)

public:
	HMapObjectModifyTool* m_pObjTool;

public:
	HMapObjectPane();
	virtual ~HMapObjectPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


