#include "HObjectManager.h"

HObject* HObjectManager::Add(HObject* pAddObject)
{
	if (pAddObject == nullptr) return nullptr;

	HObject* pData = GetPtr(pAddObject->m_szName);
	if (pData != nullptr)
	{
		return pData;
	}
	m_List.insert(make_pair(pAddObject->m_szName, pAddObject));
	return nullptr;
}

HObject* HObjectManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}


HObject* HObjectManager::Load(const TCHAR* filename, DWORD dwMaskColor)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	// 스트링을 나누는 함수
	_wsplitpath_s(filename, drive, dir, name, ext);

	wstring DIR = dir;
	wstring key;
	wstring FileLoad;
	wstring maskFile;

	if (DIR.empty())
	{
		FileLoad = m_BtDefault;
		FileLoad += filename;

		maskFile = m_BtDefault;
		maskFile += name;
		maskFile += L"mask";
		maskFile += +ext;

		key = filename;

	}
	else
	{
		FileLoad = filename;
		maskFile = drive;
		maskFile += dir;
		maskFile += name;
		maskFile += L"mask";
		maskFile += ext;

		key += name;
		key += ext;
	}
	HObject* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}


	pData = new HObject;
	pData->Init();
	if (pData->Load(FileLoad.c_str(), maskFile.c_str(), dwMaskColor))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
}

bool HObjectManager::Init()
{
	return true;
}

bool HObjectManager::Frame()
{
	return true;
}

bool HObjectManager::Render()
{
	return true;
}

bool HObjectManager::Release()
{
	for (m_iter = m_List.begin(); m_iter != m_List.end(); m_iter++)
	{
		delete (*m_iter).second;
	}

	m_List.clear();
	return true;
}

HObjectManager::HObjectManager()
{
	m_BtDefault = L"../../Data/Bitmap/";
}

HObjectManager::~HObjectManager()
{
	Release();
}