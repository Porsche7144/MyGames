#pragma once
#include "HStd.h"
#include "HMatrix.h"

class HCamera
{
public:
	HVector3					m_vCameraPos = { 10,0,-10 };
	HVector3					m_vCameraTarget = { 0,0,0 };
	HMatrix						m_matView;
	HMatrix						m_matProject;

	HVector3 m_vLook;
	HVector3 m_vUp;
	HVector3 m_vRight;

	float m_fSpeed;
	float m_fDistance;

public:
	virtual bool Frame();
	virtual bool CreateViewMatrix(HVector3 p, HVector3 t, HVector3 u = { 0,1,0 });
	virtual void UpdateVector();
	virtual bool CreateProjectionMatrix(float fN, float fF, float fFov, float fAspect);
	virtual void SetPos(HVector3 p);
	virtual void SetTarget(HVector3 p);
	virtual void Update(HVector4 d);

	void FrontMovement(float fDir = 1.0f);
	void RightMovement(float fDir = 1.0f);
	void UpMovement(float fDir);
	void FrontBase(float fDir);
	void RightBase(float fDir);
	void UpBase(float fDir);

public:
	HCamera();
	virtual ~HCamera();
};

