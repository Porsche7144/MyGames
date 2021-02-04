#include "HSoundManager.h"


HSound* HSoundManager::Load(const char* filename)
{
	CHAR drive[MAX_PATH] = { 0, };
	CHAR dir[MAX_PATH] = { 0, };
	CHAR name[MAX_PATH] = { 0, };
	CHAR ext[MAX_PATH] = { 0, };
	// 스트링을 나누는 함수
	_splitpath_s(filename, drive, dir, name, ext);

	string DIR = dir;
	string key;
	string FileLoad;		

	if (DIR.empty())
	{
		FileLoad = m_Default;
		FileLoad += filename;
		key = filename;
	}
	else
	{
		FileLoad = filename;
		key += name;
		key += ext;
	}
	HSound* pData = GetPtr(key);
	if(pData != nullptr)
	{
		return pData;
	}


	pData = new HSound;
	pData->Init();
	if (pData->Load(m_pSystem, FileLoad))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	return nullptr;
}

bool HSoundManager::PlayEffect(const char* filename, bool bLoop)
{
	HSound* pSound = GetPtr(filename);
	if (pSound != nullptr)
	{
		pSound->PlayEffect(bLoop);
		return true;
	}
	return false;
}

HSound* HSoundManager::GetPtr(string filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool HSoundManager::Init()
{
	FMOD_RESULT hr = FMOD::System_Create(&m_pSystem);
	if (hr != FMOD_OK)
	{
		return false;
	}
	if (m_pSystem != nullptr)
	{
		hr = m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
		if (hr != FMOD_OK)
		{
			return false;
		}
	}
	return true;
}
bool HSoundManager::Frame()
{
	m_pSystem->update();
	return true;
}
bool HSoundManager::Render()
{
	return true;
}
bool HSoundManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();

	m_pSystem->close();
	m_pSystem->release();
	return true;
}

HSoundManager::HSoundManager()
{
	m_Default = "../../Data/Sound/";
}

HSoundManager::~HSoundManager()
{

}