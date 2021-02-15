#include "HVector.h"

Vector3::Vector3()
{
	x = y = z = 0.0f;
}

Vector3::Vector3(const Vector3& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
}

Vector3::Vector3(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

// 연산자 재정의 
Vector3 Vector3::operator + (Vector3 const &v0)
{
	return Vector3(v0.x + x, v0.y + y, v0.z + z);
}

Vector3 Vector3::operator - (Vector3 const &v0)
{
	return Vector3(x - v0.x, y - v0.y, z - v0.z);
}

Vector3 Vector3::operator + (float const &fScala)
{
	return Vector3(fScala + x, fScala + y, fScala + z);
}
Vector3 Vector3::operator * (float const &fScala)
{
	return Vector3(fScala*x, fScala*y, fScala*z);
}
Vector3 Vector3::operator / (float const &fScala)
{
	float fInvert = 1.0f / fScala;
	return Vector3(x*fInvert, y*fInvert, z*fInvert);
}

Vector3 Vector3::operator += (Vector3 const &v0)
{
	x += v0.x;
	y += v0.y;
	z += v0.z;
	return *this;
}

// Dot Product
float Vector3::operator | (Vector3 const &v0)
{
	return x * v0.x + y * v0.y + z * v0.z;
}

// Cross Product
Vector3 Vector3::operator ^ (Vector3 const &v0)
{
	return Vector3((y*v0.z - z * v0.y), (z*v0.x - x * v0.z), (x*v0.y - y * v0.x));
}

bool	Vector3::operator == (Vector3 const &v0)
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
float Vector3::LengthSquared()
{
	return (x*x + y * y + z * z);
}

// 원점으로 부터의 거리
float Vector3::Length()
{
	return (float)sqrt(LengthSquared());
}

Vector3 Vector3::Normal()
{
	float invertLength = 1.0f / Length();
	return Vector3(x*invertLength, y*invertLength, z*invertLength);
}

float Vector3::Angle(Vector3& v0)
{
	float fLength1 = Length();
	float fLength2 = v0.Length();
	float fCosAngle = ((*this) | v0) / (fLength1*fLength2);
	return (float)RadianToDegree(acos(fCosAngle));
}


Vector4::Vector4()
{
	x = y = z = w = 0.0f;
}

float Vector4::Length()
{
	return (float)sqrt((x*x + y * y + z * z + w * w));
}

float Vector4::operator | (Vector4 const &v0)
{
	return x * v0.x + y * v0.y + z * v0.z + w * v0.w;
}

bool	Vector4::operator == (Vector4 const &v0)
{
	if (fabs(x - v0.x) < HBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < HBASIS_EPSILON)
		{
			if (fabs(z - v0.z) < HBASIS_EPSILON)
			{
				if (fabs(w - v0.w) < HBASIS_EPSILON)
				{
					return true;
				}
			}

		}
	}
	return false;
}

Vector4 Vector4::Normal()
{
	float invertLength = 1.0f / Length();
	return Vector4(x*invertLength, y*invertLength, z*invertLength, w*invertLength);
}

Vector4::Vector4(const Vector4& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
	w = v0.w;
}



Vector4::Vector4(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}


Vector2::Vector2()
{
	x = y = 0.0f;
}

bool	Vector2::operator == (Vector2 const &v0)
{
	if (fabs(x - v0.x) < HBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < HBASIS_EPSILON)
		{
			return true;
		}
	}
	return false;
}

Vector2::Vector2(const Vector2& v0)
{
	x = v0.x;
	y = v0.y;
}

Vector2::Vector2(float fX, float fY)
{
	x = fX;
	y = fY;
}