#pragma once
#include "HCamera.h"
#include "HFrustum.h"

class HModelViewCamera : public HCamera
{
public:
	HFrustum m_Frustum;

public:
	Vector3 m_vDownPt;
	Vector3 m_vCurrentPt;
	Quaternion m_qNow;
	Quaternion m_qDown;
	Matrix m_mModelLastRot;

public:
	int WndProc(HWND hWnd, UINT message, WPARAM wParam,	LPARAM lParam);
	virtual void Update(Vector4 d);
	Matrix GetRotationMatrix();
	Quaternion QuatFromPoints(Vector3 v0, Vector3 v1);
	Vector3 ScreenToVector(float px, float py);
	void OnBegin(int x, int y);
	void OnMove(int x, int y);
	void OnEnd();

public:
	virtual bool Frame();

public:
	HModelViewCamera();
	virtual ~HModelViewCamera();
};

