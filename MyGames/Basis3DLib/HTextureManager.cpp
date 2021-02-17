#include "HTextureManager.h"

HTexture*    HTextureManager::Load(ID3D11Device* pd3dDevice, const TCHAR* filename)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	std::wstring Dir = dir;
	std::wstring key;
	std::wstring loadfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}
	HTexture* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}


	pData = new HTexture;
	pData->Init();
	if (pData->Load(pd3dDevice, loadfile))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;

	return nullptr;
}

HTexture*    HTextureManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool		HTextureManager::Init()
{
	return true;
}
bool		HTextureManager::Frame()
{
	return true;
}
bool		HTextureManager::Render()
{
	return true;
}
bool		HTextureManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}

HTextureManager::HTextureManager()
{
	m_szDefaultPath = L"../../data/bitmap/";
}

HTextureManager::~HTextureManager()
{
	Release();
}