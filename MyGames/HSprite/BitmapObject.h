#pragma once
#include "BitmapManager.h"

class BitmapObject
{
public:
	HBitmap*	m_pOriginBmp;
	HBitmap*	m_pMaskBmp;
	RECT		m_rtSrc;
	RECT		m_rtDesk;
	HPoint		m_ptPos;
	bool		m_bBmpKey;
	DWORD		m_dwBmpKey;
	float		m_fSpeed;
	vector<HBitmap*> m_StateBitmap;

public:
	bool Load(const TCHAR* Original, const TCHAR* Mask = nullptr);
	bool Load(const TCHAR* Original, DWORD dwMask);
	void Set(RECT rtSrc, RECT rtDesk, float fSpeed = 200.0f);
	void Set(HPoint pos, RECT rtDesk, float fSpeed = 200.0f);
	void SetPosition(HPoint p);
	void SetPos(HPoint p);
	bool DrawKey();

	virtual bool Init();
	virtual bool Render();
	virtual bool Frame();
	virtual bool Release();

public:
	BitmapObject();
	virtual ~BitmapObject();
};

