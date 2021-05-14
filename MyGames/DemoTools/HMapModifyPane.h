#pragma once

#include "HMapModifyTool.h"
// HMapModifyPane

class HMapModifyPane : public CDockablePane
{
	DECLARE_DYNAMIC(HMapModifyPane)

public:
	HMapModifyTool* m_wmModify;
public:
	HMapModifyPane();
	virtual ~HMapModifyPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


