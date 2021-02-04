#include "HNetStd.h"

class HNetUser
{
public:
	SOCKET		m_Sock;
	SOCKADDR_IN addr;
	char recvBuf[10000];
	int iRecvSize;
	int iSendSize;
	T_STR szName;
	std::vector<UPACKET>	m_SendPacket;

	HNetUser();
	virtual ~HNetUser();

};