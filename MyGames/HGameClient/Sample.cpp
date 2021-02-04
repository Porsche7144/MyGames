#include "Sample.h"

bool  Sample::Init()
{
	if (m_Network.InitNetwork("192.168.0.154", 10000) == false)
	{
		return false;
	}
	return true;
}

bool  Sample::Frame()
{
	EnterCriticalSection(&m_Network.m_cs);
	m_Network.RecvData(m_User);
	LeaveCriticalSection(&m_Network.m_cs);
	return true;
}

bool  Sample::Render()
{
	m_Network.PacketProcess();

	HChatMsg chatmsg;
	memset(&chatmsg, 0, sizeof(chatmsg));
	strcpy_s(chatmsg.szName, "Lee");
	strcpy_s(chatmsg.buffer, "SH");
	chatmsg.iCnt = 1;

	if (m_Network.SendMsg(m_Network.m_Sock, (char*)&chatmsg, sizeof(HChatMsg), PACKET_CHAT_MSG) < 0)
	{
		return false;
	}

	return true;
}

bool Sample::Release()
{
	m_Network.DeleteNetwork();
	return true;
}

GAMERUN;