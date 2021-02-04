#pragma once
#include "Hcore.h"
#include "fmod.h" 
#include "fmod_errors.h"
#include "fmod.hpp"

using namespace FMOD;

class HSound
{
public:
	FMOD::System*			m_pSystem;
	FMOD::Sound*			m_pSound;
	FMOD::Channel*			m_pChannel;
	TCHAR					m_szBuffer[256];

public:
	bool  Load(System* pSystem, std::string szFileName, bool bPaused = true);
	void  Stop();
	bool  Play(bool bPaused = false, bool bLoop = true);
	bool  PlayEffect(bool bLoop = false);
	void  Paused();
	void  Volume(float fVolume);
	FMOD_RESULT SetMode(FMOD_MODE mode = FMOD_LOOP_OFF);

public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();

public:
	HSound();
	~HSound();
};

