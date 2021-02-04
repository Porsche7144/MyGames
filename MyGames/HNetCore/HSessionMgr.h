#pragma once
#include "HNetUser.h"
#include "HThread.h"

class HSessionMgr : public HNetSingleton<HSessionMgr>,
					public HThread
{
private:
	friend class HNetSingleton<HSessionMgr>;

public:
	std::vector<UPACKET> m_BroadCastingPacketPool;
	std::map<SOCKET, HNetUser*>		m_UserList;

public:
	virtual bool AddUser(HNetUser* pUser);
	virtual bool DelUser(HNetUser* user);
	virtual bool CloseUser(HNetUser* pUser);

public:
	virtual bool Run();
	HSessionMgr();
	virtual ~HSessionMgr();
};

#define I_Sesstion HSessionMgr::GetInstance()
