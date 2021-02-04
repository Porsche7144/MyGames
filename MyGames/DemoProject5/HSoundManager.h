#pragma once
#include "HSound.h"

class HSoundManager : public HSingleton<HSoundManager>
{
public:
	friend  class HSingleton<HSoundManager>;
private:
	FMOD::System*						m_pSystem;
	std::map<string, HSound*>			m_List;
	std::map<string, HSound*>::iterator	m_iter;
	string								m_Default;

public:
	HSound*    Load(const char* filename);
	HSound*    GetPtr(string filename);
	bool	   PlayEffect(const char* filename, bool bLoop);

	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	HSoundManager();
public:
	~HSoundManager();
};

#define g_SoundMgr HSoundManager::GetInstance()