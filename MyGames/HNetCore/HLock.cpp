#include "HLock.h"

HLock::HLock(HServerObj * pObj) : m_pServerObj(pObj)
{
	if (m_pServerObj == nullptr) return;
	EnterCriticalSection(&m_pServerObj->m_cs);
}

HLock::~HLock()
{
	if (m_pServerObj == nullptr) return;
	LeaveCriticalSection(&m_pServerObj->m_cs);
}
