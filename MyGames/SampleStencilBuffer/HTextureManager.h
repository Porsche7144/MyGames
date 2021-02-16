#pragma once
#include "HTexture.h"
class HTextureManager : public HSingleton<HTextureManager>
{
public:
	friend  class HSingleton<HTextureManager>;
private:
	std::map<wstring, HTexture*>				m_List;
	std::map<wstring, HTexture*>::iterator	m_iter;
	std::wstring				m_szDefaultPath;
public:
	HTexture*		CreateLoad(LONG iWidth, LONG iHeight, const TCHAR* filename, int iBitCount = 24);
	HTexture*		Load(const TCHAR* filename);
	HTexture*		GetPtr(wstring filename);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	HTextureManager();
public:
	~HTextureManager();
};
#define g_BitmapMgr HTextureManager::GetInstance()
