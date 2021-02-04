#pragma once
#include "BitmapManager.h"
#include "HCollision.h"

struct HPoint
{
	float x;
	float y;
};

enum ObjState
{
	BACKGROUND = 0,
	UI,
	USER,
	NPC
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

enum HControlState
{
	DEFAULT = 0,
	PUSH,
	SELECT,
	DISABLE

};

class BitmapObject
{
public:
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

public:
	virtual bool Init();
	virtual bool Render();
	virtual bool Frame();
	virtual bool Release();

	virtual bool Load(const TCHAR* Original, const TCHAR* Mask = nullptr);
	virtual bool Load(const TCHAR* Original, DWORD dwMask);
	virtual void Set(RECT rtSrc, RECT rtDesk, float fSpeed = 200.0f);
	virtual void SetPosition(HPoint p);
	virtual void DrawKey();

public:
	BitmapObject();
	virtual ~BitmapObject();
};

