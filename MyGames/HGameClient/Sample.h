#pragma once
#include "HNetworkClient.h" // Hcore 안에 windows 헤더가 있으므로 반드시 winsock을 먼저 불러온다.
#include "Hcore.h"

class Sample : public Hcore
{
	HNetworkClient	m_Network;
	HNetUser			m_User;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

};

