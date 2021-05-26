#pragma once


// HPropertiesWnd

class HPropertiesWnd : public CDockablePane
{
	DECLARE_DYNAMIC(HPropertiesWnd)

public:
	HPropertiesWnd();
	virtual ~HPropertiesWnd();
	CMFCPropertyGridCtrl m_wndPropList;
	void InitPropList();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
};


