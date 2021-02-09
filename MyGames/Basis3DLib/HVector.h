#pragma once
#include <Windows.h>
#include <math.h>

#define HBASIS_EPSILON		((FLOAT)  0.001f)
#define HBASIS_PI			((FLOAT)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (HBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / HBASIS_PI))
#define MAKECOLOR_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

namespace HBASIS_VECTOR {

	struct float2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};

	struct float3
	{
		union
		{
			struct { float x, y, z; };
			float v[3];
		};
	};
	struct float4
	{
		union
		{
			struct { float x, y, z, w; };
			float v[4];
		};
	};
	class HVector3 : public float3
	{
	public:
		HVector3();
		HVector3(const HVector3& v0);
		HVector3(float fX, float fY, float fZ);
		// 연산자 재정의 
		HVector3 operator + (HVector3 const &v0);
		HVector3 operator - (HVector3 const &v0);
		HVector3 operator * (float const &fScala);
		HVector3 operator + (float const &fScala);
		HVector3 operator / (float const &fScala);
		// Dot Product
		float operator | (HVector3 const &v0);
		// Cross Product
		HVector3 operator ^ (HVector3 const &v0);
		bool	operator == (HVector3 const &v0);
		// 제곱
		float LengthSquared();
		// 원점으로 부터의 거리
		float Length();
		HVector3 Normal();
		float Angle(HVector3& v0);
	};
	class HVector4 : public float4
	{
	public:
		HVector4();
		HVector4(const HVector4& v0);
		HVector4(float fX, float fY, float fZ, float fW);
		float Length();
		HVector4 Normal();
		float operator | (HVector4 const &v0);
		bool operator == (HVector4 const &v0);
	};

	class HVector2 : public float2
	{
	public:
		HVector2();
		HVector2(const HVector2& v0);
		HVector2(float fX, float fY);
		bool operator == (HVector2 const &v0);
	};
}
using namespace HBASIS_VECTOR;