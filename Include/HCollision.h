#pragma once
#include "HStd.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

struct H_SPHERE
{
	Vector3 vCenter;
	float fRadius;
};

struct H_BOX
{
	// Common
	Vector3 vCenter;
	Vector3 vPos[8];

	// AABB
	Vector3 vMax;
	Vector3 vMin;

	// OBB
	Vector3 vAxis[3];
	float fExtent[3];

};

class HCollision
{
public:
	static bool SphereToSphere(H_SPHERE rt1, H_SPHERE rt2);
	static bool RectInPt(RECT rt, POINT pt);
	static bool Rect2Rect(RECT rt1, RECT rt2);
};

