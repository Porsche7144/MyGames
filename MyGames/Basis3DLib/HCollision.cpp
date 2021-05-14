#include "HCollision.h"

bool HCollision::SphereToSphere(H_SPHERE rt1, H_SPHERE rt2)
{
	float fDistance = (rt1.vCenter - rt2.vCenter).Length();
	if (rt1.fRadius + rt2.fRadius > fDistance)
	{
		return true;
	}
	return false;
}

bool HCollision::RectInPt(RECT rt, POINT pt)
{
	return (pt.x >= rt.left && 
			pt.x <= rt.right &&
			pt.y >= rt.top && 
			pt.y <= rt.bottom);
}

bool HCollision::Rect2Rect(RECT rtDesk, RECT rtSrc)
{
	// °¡·Î
	POINT  cDesk, cSrc, Radius, DeskRadius, SrcRadius;
	cDesk.x = (rtDesk.left + rtDesk.right) / 2;
	cDesk.y = (rtDesk.top + rtDesk.bottom) / 2;
	cSrc.x = (rtSrc.left + rtSrc.right) / 2;
	cSrc.y = (rtSrc.top + rtSrc.bottom) / 2;
	Radius.x = abs(cDesk.x - cSrc.x);
	Radius.y = abs(cDesk.y - cSrc.y);
	DeskRadius.x = (rtDesk.right - rtDesk.left) / 2;
	DeskRadius.y = (rtDesk.bottom - rtDesk.top) / 2;
	SrcRadius.x = (rtSrc.right - rtSrc.left) / 2;
	SrcRadius.y = (rtSrc.bottom - rtSrc.top) / 2;
	if (Radius.x <= (DeskRadius.x + SrcRadius.x) &&
		Radius.y <= (DeskRadius.y + SrcRadius.y))
	{
		return true;
	}
	return false;
}