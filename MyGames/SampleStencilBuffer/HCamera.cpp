#include "HCamera.h"

bool HCamera::Frame()
{
	HVector3 vUp = { 0,1,0 };
	m_matView.CreateViewLook(m_vCameraPos, m_vCameraTarget, vUp);
	UpdateVector();

	return true;
}

bool HCamera::CreateViewMatrix(HVector3 p, HVector3 t, HVector3 u)
{
	m_vCameraPos = p;
	m_vCameraTarget = t;
	m_fDistance = (m_vCameraPos - m_vCameraTarget).Length();
	m_matView.CreateViewLook(p, t, u);

	UpdateVector();

	return true;
}

void HCamera::UpdateVector()
{
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;
	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;
	m_vRight.x = m_matView._11;
	m_vRight.y = m_matView._21;
	m_vRight.z = m_matView._31;
}

bool HCamera::CreateProjectionMatrix(float fN, float fF, float fFov, float fAspect)
{
	m_matProject.PerspectiveFovLH(fN, fF, fFov, fAspect);
	return true;
}

void HCamera::SetPos(HVector3 p)
{
	m_vCameraPos = p;
}

void HCamera::SetTarget(HVector3 p)
{
	m_vCameraTarget = p;
}

void HCamera::Update(HVector4 d)
{
	HMatrix matRotation;
	matRotation.YRotate(d.y);
	HVector3 vLocalUp = { 0,1,0 };
	HVector3 vLocalLook = { 0,0,1 };
	vLocalUp = matRotation * vLocalUp;
	vLocalLook = matRotation * vLocalLook;
	vLocalLook.Normal();
	vLocalUp.Normal();
	float fHeight = m_vCameraPos.y;
	m_vCameraPos = m_vCameraTarget - vLocalLook * m_fDistance;
	m_vCameraPos.y = fHeight;
}

void HCamera::FrontMovement(float fDir)
{
	HVector3 vMove = m_vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
}

void HCamera::RightMovement(float fDir)
{
	HVector3 vMove = m_vRight * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
}

void HCamera::UpMovement(float fDir)
{
	HVector3 vMove = m_vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
}

void HCamera::FrontBase(float fDir)
{
	HVector3 vLook = { 0,0,1 };
	HVector3 vMove = vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_fSpeed;
}

void HCamera::RightBase(float fDir)
{
	HVector3 vSide = { 1,0,0 };
	HVector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_fSpeed;
}

void HCamera::UpBase(float fDir)
{
	HVector3 vUp = { 0,1,0 };
	HVector3 vMove = vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_fSpeed;
}

HCamera::HCamera()
{
	m_fSpeed = 30.0f;
}

HCamera::~HCamera()
{
}

