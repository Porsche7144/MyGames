#include "BitmapManager.h"

HBitmap* BitmapManager::Load(const TCHAR* filename)
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

	if (DIR.empty())
	{
		FileLoad = m_BtDefault;
		FileLoad += filename;
		key = filename;
	}
	else
	{
		FileLoad = filename;
		key += name;
		key += ext;
	}
	 m_pBitData = GetPtr(key);
	if (m_pBitData != nullptr)
	{
		return m_pBitData;
	}


	m_pBitData = new HBitmap;
	m_pBitData->Init();
	if (m_pBitData->Load(FileLoad))
	{
		m_List.insert(make_pair(key, m_pBitData));
		return m_pBitData;
	}

	return nullptr;
}

HBitmap* BitmapManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool BitmapManager::Init()
{
	return true;
}
bool BitmapManager::Frame()
{
	return true;
}
bool BitmapManager::Render()
{
	return true;
}
bool BitmapManager::Release()
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

BitmapManager::BitmapManager()
{
	m_BtDefault = L"../../Data/Bitmap/";
}

BitmapManager::~BitmapManager()
{

}