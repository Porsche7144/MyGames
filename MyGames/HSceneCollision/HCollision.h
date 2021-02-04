#pragma once
#include "Hstd.h"

class HCollision
{
public:
	static bool RectPt(RECT rt, POINT pt);
	static bool RectToRect(RECT rtDesk, RECT rtSrc);
};

