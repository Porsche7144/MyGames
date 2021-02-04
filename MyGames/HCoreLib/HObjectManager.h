#pragma once
#include "HEffect.h"
class HObjectManager : public HSingleton<HObjectManager>
{
public:
	friend  class HSingleton<HObjectManager>;
private:
	std::map<wstring, HObject*>				m_List;
	std::map<wstring, HObject*>::iterator	m_iter;
	wstring									m_BtDefault;

public:
	HObject* Add(HObject* pAddObject);
	HObject* Load(const TCHAR* filename, DWORD dwMaskColor = RGB(255, 0, 255));
	HObject* GetPtr(wstring filename);

	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
private:
	HObjectManager();
public:
	virtual ~HObjectManager();
};


#define g_ObjMgr HObjectManager::GetInstance()
