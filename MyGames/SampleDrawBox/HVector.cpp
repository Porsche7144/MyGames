#include "HVector.h"

HVector3::HVector3()
{
	x = y = z = 0.0f;
}

HVector3::HVector3(const HVector3& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
}

HVector3::HVector3(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}
