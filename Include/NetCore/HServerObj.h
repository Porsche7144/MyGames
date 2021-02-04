#pragma once
#include "HNetStd.h"
class HServerObj
{
public:
	CRITICAL_SECTION m_cs;
public:
	HServerObj();
	virtual ~HServerObj();
};
