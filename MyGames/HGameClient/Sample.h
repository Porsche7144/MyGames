#pragma once
#include "HNetworkClient.h" // Hcore �ȿ� windows ����� �����Ƿ� �ݵ�� winsock�� ���� �ҷ��´�.
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

