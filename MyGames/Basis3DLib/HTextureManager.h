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
	HTexture* m_pWhiteTexture;

public:
	HTexture*		Load(ID3D11Device* pd3dDevice, const TCHAR* filename);
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
#define g_TextureMgr HTextureManager::GetInstance()
