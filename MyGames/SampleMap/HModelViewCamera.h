#pragma once
#include "HCamera.h"
#include "HFrustum.h"

class HArcBall
{
public:
	Vector3 m_vDownPt;
	Vector3 m_vCurrentPt;
	Quaternion m_qNow;
	Quaternion m_qDown;
	bool	m_bDrag;

public:
	Matrix GetRotationMatrix();
	Quaternion QuatFromPoints(Vector3 v0, Vector3 v1);
	Vector3 ScreenToVector(float px, float py);
	void OnBegin(int x, int y);
	void OnMove(int x, int y);
	void OnEnd();

public:
	HArcBall() 
	{
		m_qDown = Quaternion::Identity;
		m_qNow = Quaternion::Identity;
		m_bDrag = false;
	};
	virtual ~HArcBall() {};
};

class HModelViewCamera : public HCamera
{
public:
	HFrustum m_Frustum;

public:
	HArcBall m_WorldArcBall;
	HArcBall m_ViewArcBall;
	Matrix m_mModelLastRot;

public:
	int WndProc(HWND hWnd, UINT message, WPARAM wParam,	LPARAM lParam);
	virtual void Update(Vector4 d) override;

public:
	virtual bool PostInit() override;
	virtual bool Frame() override;

public:
	HModelViewCamera();
	virtual ~HModelViewCamera();
};

