#pragma once
#include "TObject.h"
#include "TInput.h"
class TUserInterface : public TObject
{
public:
	int   m_iState;
	std::vector<TUserInterface>  m_list;
public:
	virtual LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		return -1;
	};
	virtual TObject* Clone() override;
	TUserInterface() : m_iState(0)
	{
		m_iObjectType = 300;
	}
};

class TButton : public TUserInterface
{
public:
	bool	m_bClick;
public:
	bool   Frame();
	bool   Render();
	virtual TObject* Clone() override;
public:
	TButton()
	{
		m_iObjectType = 301;
		m_bClick = false;
	}
};
class TEdit : public TUserInterface
{
public:
	T_STR	m_szEdit;
	int		m_iWindowCtrl;
	static int g_iControlID;
	bool	m_bClick;
	HWND	m_hWndEdit;
	RECT	m_rtWndEditID;
	bool	m_bEditFocus = false;
public:
	void   CreateEdit();
public:
	virtual TObject* Clone() override;
	virtual void  Set(RECT rtSrc, RECT rtDesk) override;
	bool	Init() override;
	bool	Frame()override;
	bool	Render()override;
	LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	TEdit()
	{
		m_iObjectType = 302;
		m_bClick = false;
	}
};