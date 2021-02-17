#pragma once
#include "HObject.h"
#include "HInput.h"

struct HOffset
{
	float fScaleX0;
	float fScaleX1;
	float fScaleY0;
	float fScaleY1;
	float fOffsetU0;
	float fOffsetV0;
	float fOffsetU1;
	float fOffsetV1;
};

struct HRect
{
	float    left;
	float    top;
	float    right;
	float    bottom;
	void operator = (const RECT rt)
	{
		left = (float)rt.left;
		top = (float)rt.top;
		right = (float)rt.right;
		bottom = (float)rt.bottom;
	}
};

struct HIndex
{
	int x, y;
	bool operator == (const HIndex& pos)
	{
		return (x == pos.x && y == pos.y);
	}
	HIndex operator + (const HIndex& right_)
	{
		return{ x + right_.x, y + right_.y };
	}
	HIndex(int fX, int fY)
	{
		x = fX;
		y = fY;
	}
	HIndex()
	{
		x = y = 0;
	}
};

struct PT_VERTEX
{
	HPoint p;
	HPoint t;
	bool operator == (const PT_VERTEX & Vertex)
	{
		if (p == Vertex.p  &&  t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PT_VERTEX() {}
	PT_VERTEX(HPoint		vp, HPoint     vt)
	{
		p = vp, t = vt;
	}
};

class HUserInterface : public HObject
{
public:

	std::vector<HUserInterface>  m_list;
public:
	virtual LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual HObject* Clone() override;
	virtual bool	PostFrame() override;
	HUserInterface()
	{
		m_iObjectType = 300;
	}
};
class HActor : public HUserInterface
{
public:
	HRect		m_rtInit;
	HOffset		m_tOffset;
	HPoint		m_vCenter;
	PT_VERTEX	m_VertexList[6];
public:
	virtual HPoint  Gen(float x, float y);
	virtual void    GenCenter();
	virtual void    SetVertexData();
	virtual  bool	SetData(float left, float top, float width, float height);

public:
	HActor() {};
	~HActor() {}
};
class HDialogBox : public HActor
{
public:
	vector<PT_VERTEX> m_tVertexList;
	HActor             m_Actor[9];
	HPoint			   m_cpPoint[16];
public:
	void   SetVertexData();
	bool   SetData(RECT rt, float fScaleX0, float fScaleX1,
		float fScaleY0, float fScaleY1,
		float fOffsetU0, float fOffsetU1,
		float fOffsetV0, float fOffsetV1);
public:
	virtual bool	Init() override;
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext) override;
	virtual void  Set(RECT rtSrc, RECT rtDesk) override;
	virtual void  Set(HPoint p, RECT rtDesk) override;
public:
	HDialogBox() {};
	~HDialogBox() {}
};
class HButton : public HUserInterface
{
public:

public:
	bool   Frame();
	bool   Render(ID3D11DeviceContext*	pd3dContext);
	virtual HObject* Clone() override;
public:
	HButton()
	{
		m_iObjectType = 301;
	}
};
class HEdit : public HUserInterface
{
public:
	T_STR	m_szEdit;
	int		m_iWindowCtrl;
	static int g_iControlID;
	HWND	m_hWndEdit;
	RECT	m_rtWndCtrl;
	bool	m_bEditFocus = false;
public:
	void   CreateEdit();
public:
	virtual HObject* Clone() override;
	bool	Init() override;
	bool	Frame()override;
	bool	Render(ID3D11DeviceContext*	pd3dContext)override;
	LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	HEdit()
	{
		m_iObjectType = 302;
	}
};