#pragma once
#ifndef HBASIS2D_MATH
#define HBASIS2D_MATH
#include <math.h>
#include <time.h>


	#define HBASIS_EPSILON		((float)  0.001f)
	#define HBASIS_PI			((float)  3.141592654f)
	#define DegreeToRadian( degree ) ((degree) * (HBASIS_PI / 180.0f))
	#define RadianToDegree( radian ) ((radian) * (180.0f / HBASIS_PI))
	#define randf(x) (x*rand()/(float)RAND_MAX)
	#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
	#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
	#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

namespace HBASIS_CORE_LIB
{
	static const float H_EPSILON = 0.001f;
	static const float H_PI = 3.141592f;
	struct float2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};
	struct HPoint : float2
	{
		static HPoint zero;
		HPoint() = default;
		HPoint(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		float* get()
		{
			return v;
		}
		void operator = (float* arr)
		{
			x = arr[0];
			y = arr[1];
		}
		float& operator [] (int index)
		{
			if (index == 0) return x;
			return y;
		}
		bool operator == (HPoint p)
		{
			if (fabs(x - p.x) < H_EPSILON)
			{
				if (fabs(y - p.y) < H_EPSILON)
				{
					return true;
				}
			}
			return false;
		}
		bool operator != (HPoint p)
		{
			if (fabs(x - p.x) < H_EPSILON)
			{
				if (fabs(y - p.y) < H_EPSILON)
				{
					return false;
				}
			}
			return true;
		}
		HPoint operator * (float fValue)
		{
			HPoint ret;
			ret.x = x * fValue;
			ret.y = y * fValue;
			return ret;
		}		
		HPoint operator + (HPoint p)
		{
			HPoint ret;
			ret.x = x + p.x;
			ret.y = y + p.y;
			return ret;
		}
		HPoint operator - (HPoint p)
		{
			HPoint ret;
			ret.x = x - p.x;
			ret.y = y - p.y;
			return ret;
		}
		float Length()
		{
			return sqrt(x*x + y * y);
		}

		HPoint Normalize()
		{
			float fD = Length();
			if (fD != 0.0f)
			{
				x /= fD;
				y /= fD;
			}
			return HPoint(x, y);
		}

		float Angle(HPoint& v0)
		{
			HPoint vA = Normalize();
			HPoint vB = v0.Normalize();
			float fDot = (vA | vB);
			if (fDot > 1.0f) fDot = 1.0f;
			float fRadian = acos(fDot);
			float fAngle = (float)RadianToDegree(fRadian);
			if (fDot < 0.0f)
			{
				fAngle = 180.0f + (180.0f - fAngle);
			}
			else
			{
				fAngle = fAngle;
			}
			return fAngle;
		}
		// Dot Product
		float operator | (HPoint const &v0)
		{
			return x * v0.x + y * v0.y;
		}
		// Cross Product
		HPoint operator ^ (HPoint const &v0)
		{
			if (*this == v0)
			{
				return v0;
			}
			return HPoint(0, (x*v0.y - y * v0.x));
		}

		bool   CCW(HPoint vLook, HPoint vDir)
		{
			//HPoint pTarget, pPos;
			//// Y값은 원점에서 아래로 감소하도록 클라이언트 좌표를 직각 좌표로 변환한다.
			//pTarget.y *= -1.0f;
			//pPos.y *= -1.0f;
			//// 이후 이전 프레임의 벡터와 현재의 벡터의 각을 계산한다.
			//VectoHPointr2 vDir;
			//vDir.x = pTarget.x - pPos.x;
			//vDir.y = pTarget.y - pPos.y;
			//vDir = vDir.Normal();
			//float fAngle = vDir.Angle(vLook);

			// 시계 반대방향으로 움직이면 각도를 뺀다.
			HPoint vA, vB, vC;
			vA.x = vLook.x;	vA.y = vLook.y;
			vB.x = vDir.x;	vB.y = vDir.y;
			vC = (vA ^ vB).Normalize();
			float fDot = vC | HPoint(0, -1);
			if (fDot > 0.0f)
			{
				return false;
			}
			return true;
		}
	};
	
}
namespace H_CORE = HBASIS_CORE_LIB; // 별칭 사용
using namespace H_CORE;
#endif