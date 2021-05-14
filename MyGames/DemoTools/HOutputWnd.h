#pragma once


// HOutputWnd

class HOutputList : public CListBox
{
public:
	HOutputList() {};
	virtual ~HOutputList() {};
	DECLARE_MESSAGE_MAP();
};

class HOutputWnd : public CDockablePane
{
	DECLARE_DYNAMIC(HOutputWnd)
public:
	CMFCTabCtrl	  m_wndTabs;
	HOutputList   m_wndOutputBuild;
	HOutputList   m_wndOutputDebug;
	HOutputList   m_wndOutputFind;
	void PushBuild(std::wstring data);
	void PushDebug(std::wstring data);
	void PushFind(std::wstring data);
public:
	HOutputWnd();
	virtual ~HOutputWnd();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	DECLARE_MESSAGE_MAP()
};


