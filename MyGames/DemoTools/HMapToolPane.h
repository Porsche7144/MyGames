#pragma once

#include "HMapToolForm.h"
// HMapToolPane

class HMapToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(HMapToolPane)

public:
	HMapToolPane();
	virtual ~HMapToolPane();
	HMapToolForm* m_wndForm;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};



