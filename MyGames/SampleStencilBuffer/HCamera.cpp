#include "HCamera.h"

int HCamera::WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{

		case WM_LBUTTONDOWN:
		{
			m_bDrag = true;
			m_ptClick.x = LOWORD(lParam);
			m_ptClick.y = HIWORD(lParam);
			m_rtOffset.left = 0;
			m_rtOffset.bottom = 0;
		}return 0;
		case WM_MOUSEMOVE:
		{
			if (m_bDrag)
			{
				m_rtOffset.left -= m_ptClick.x - LOWORD(lParam);
				m_rtOffset.bottom -= m_ptClick.y - HIWORD(lParam);
				m_ptClick.x = LOWORD(lParam);
				m_ptClick.y = HIWORD(lParam);
			}
		}return 0;
		case WM_LBUTTONUP:
		{
			m_bDrag = false;
			m_rtOffset.left = 0;
			m_rtOffset.bottom = 0;
		}return 0;

	}
	return -1;
}

bool HCamera::Frame()
{
	Vector3 vUp = { 0,1,0 };
	m_matView = Matrix::CreateLookAt(m_vCameraPos, m_vCameraTarget, vUp);
	UpdateVector();

	return true;
}

bool HCamera::CreateViewMatrix(Vector3 p, Vector3 t, Vector3 u)
{
	m_vCameraPos = p;
	m_vCameraTarget = t;
	m_fDistance = (m_vCameraPos - m_vCameraTarget).Length();
	m_matView = Matrix::CreateLookAt(p, t, u);

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

	m_vLook.Normalize();
	m_vUp.Normalize();
	m_vRight.Normalize();
}

bool HCamera::CreateProjectionMatrix(float fN, float fF, float fFov, float fAspect)
{
	m_matProject = Matrix::CreatePerspectiveFieldOfView(fFov, fAspect, fN, fF);
	return true;
}

void HCamera::SetPos(Vector3 p)
{
	m_vCameraPos = p;
}

void HCamera::SetTarget(Vector3 p)
{
	m_vCameraTarget = p;
}

void HCamera::Update(Vector4 d)
{
	Matrix matRotation;
	matRotation = Matrix::CreateRotationY(d.y);
	Vector3 vLocalUp = { 0,1,0 };
	Vector3 vLocalLook = { 0,0,1 };
	vLocalUp = Vector3::Transform(vLocalUp, matRotation);
	vLocalLook = Vector3::Transform(vLocalLook, matRotation);
	vLocalLook.Normalize();
	vLocalUp.Normalize();
	float fHeight = m_vCameraPos.y;
	m_vCameraPos = m_vCameraTarget - vLocalLook * m_fDistance;
	m_vCameraPos.y = fHeight;
}

void HCamera::FrontMovement(float fDir)
{
	Vector3 vMove = m_vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
}

void HCamera::RightMovement(float fDir)
{
	Vector3 vMove = m_vRight * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
}

void HCamera::UpMovement(float fDir)
{
	Vector3 vMove = m_vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
}

void HCamera::FrontBase(float fDir)
{
	Vector3 vLook = { 0,0,1 };
	Vector3 vMove = vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_fSpeed;
}

void HCamera::RightBase(float fDir)
{
	Vector3 vSide = { 1,0,0 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_fSpeed;
}

void HCamera::UpBase(float fDir)
{
	Vector3 vUp = { 0,1,0 };
	Vector3 vMove = vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_fSpeed;
}

HCamera::HCamera()
{
	m_fSpeed = 30.0f;
	m_bDrag = false;
}

HCamera::~HCamera()
{
}

