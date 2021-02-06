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

// 연산자 재정의 
HVector3 HVector3::operator + (HVector3 const &v0)
{
	return HVector3(v0.x + x, v0.y + y, v0.z + z);
}

HVector3 HVector3::operator - (HVector3 const &v0)
{
	return HVector3(x - v0.x, y - v0.y, z - v0.z);
}

HVector3 HVector3::operator + (float const &fScala)
{
	return HVector3(fScala + x, fScala + y, fScala + z);
}
HVector3 HVector3::operator * (float const &fScala)
{
	return HVector3(fScala*x, fScala*y, fScala*z);
}
HVector3 HVector3::operator / (float const &fScala)
{
	float fInvert = 1.0f / fScala;
	return HVector3(x*fInvert, y*fInvert, z*fInvert);
}

// Dot Product
float HVector3::operator | (HVector3 const &v0)
{
	return x * v0.x + y * v0.y + z * v0.z;
}

// Cross Product
HVector3 HVector3::operator ^ (HVector3 const &v0)
{
	return HVector3((y*v0.z - z * v0.y), (z*v0.x - x * v0.z), (x*v0.y - y * v0.x));
}

bool	HVector3::operator == (HVector3 const &v0)
{
	if (fabs(x - v0.x) < HBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < HBASIS_EPSILON)
		{
			if (fabs(z - v0.z) < HBASIS_EPSILON)
			{
				return true;
			}
		}
	}
	return false;
}

// 제곱
float HVector3::LengthSquared()
{
	return (x*x + y * y + z * z);
}

// 원점으로 부터의 거리
float HVector3::Length()
{
	return (float)sqrt(LengthSquared());
}

HVector3 HVector3::Normal()
{
	float invertLength = 1.0f / Length();
	return HVector3(x*invertLength, y*invertLength, z*invertLength);
}

float HVector3::Angle(HVector3& v0)
{
	float fLength1 = Length();
	float fLength2 = v0.Length();
	float fCosAngle = ((*this) | v0) / (fLength1*fLength2);
	return (float)RadianToDegree(acos(fCosAngle));
}


HVector4::HVector4()
{
	x = y = z = w = 0.0f;
}

HVector4::HVector4(const HVector4& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
	w = v0.w;
}

HVector4::HVector4(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}


HVector2::HVector2()
{
	x = y = 0.0f;
}

HVector2::HVector2(const HVector2& v0)
{
	x = v0.x;
	y = v0.y;
}

HVector2::HVector2(float fX, float fY)
{
	x = fX;
	y = fY;
}