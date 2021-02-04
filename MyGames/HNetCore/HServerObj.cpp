#include "HServerObj.h"


HServerObj::HServerObj()
{
	InitializeCriticalSection(&m_cs);
}

HServerObj::~HServerObj()
{
	DeleteCriticalSection(&m_cs);
}
