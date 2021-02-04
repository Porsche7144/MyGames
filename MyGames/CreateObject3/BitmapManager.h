#pragma once
#include "HBitmap.h"

class BitmapManager : public HSingleton<BitmapManager>
{
public:
	friend  class HSingleton<BitmapManager>;
private:
	std::map<wstring, HBitmap*>				m_List;
	std::map<wstring, HBitmap*>::iterator	m_iter;
	wstring									m_BtDefault;
	HBitmap*								m_pBitData;

public:
	HBitmap*    Load(const TCHAR* filename);
	HBitmap*    GetPtr(wstring filename);

	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
private:
	BitmapManager();
public:
	virtual ~BitmapManager();
};

#define g_BitMgr BitmapManager::GetInstance()