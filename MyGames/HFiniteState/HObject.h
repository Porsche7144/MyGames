#pragma once
#include "BitmapManager.h"
#include "HCollision.h"

enum ObjState
{
	BACKGROUND = 0,
	UI,
	USER,
	NPC
};

enum HControlState
{
	DEFAULT = 0,
	PUSH,
	SELECT,
	DISABLE

};

struct HObjAttribute
{
	int iObjState;
	wstring OriginBitmap;
	wstring MaskBitmap;
	wstring PushBitmap;
	wstring SelectBitmap;
	wstring dlsBitmap;
	HPoint  pos;
	RECT rtSrc;
	RECT rtDesk;
	bool bRGBKey;
	DWORD dwRGB;
};

class HObject
{
public:
	wstring		m_szName;

	HBitmap*	m_pOriginBmp;
	HBitmap*	m_pMaskBmp;
	RECT		m_rtCollide;
	RECT		m_rtSrc;
	RECT		m_rtDesk;
	HPoint		m_ptPos;
	bool		m_bBmpKey;
	DWORD		m_dwBmpKey;
	float		m_fSpeed;
	vector<HBitmap*> m_StateBitmap;
	bool		m_bDead;
	HPoint		m_ptDirection;
	int			m_iObjectType;

public:

	bool Load(const TCHAR* Original, const TCHAR* Mask = nullptr, DWORD dwColor = RGB(255,0,255));
	void Set(RECT rtSrc, RECT rtDesk, float fSpeed = 200.0f);
	void Set(HPoint pos, RECT rtDesk, float fSpeed = 200.0f);
	void SetPosition(HPoint p);
	void SetPos(HPoint p);
	bool DrawKey();

	virtual void SetTransition(DWORD dwEvent) {};

	virtual bool Init();
	virtual bool Render();
	virtual bool Frame();
	virtual bool Release();

public:
	HObject();
	virtual ~HObject();
};

