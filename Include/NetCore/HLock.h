#pragma once
#include "HServerObj.h"

class HLock
{
private:
	HServerObj*		m_pServerObj;
public:
	HLock(HServerObj* pObj);
	virtual ~HLock();
};

