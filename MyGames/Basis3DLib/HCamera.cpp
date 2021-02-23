#include "HCamera.h"

int HCamera::WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	HRESULT hitTest;
	switch (message)
	{

		case WM_LBUTTONDOWN:
		{
			m_bDrag = true;
			m_ptClick.x = LOWORD(lParam);
			m_ptClick.y = HIWORD(lParam);
			m_rtOffset.left = 0;
			m_rtOffset.bottom = 0;
		}break;
		case WM_MOUSEMOVE:
		{
			if (m_bDrag)
			{
				m_rtOffset.left -= m_ptClick.x - LOWORD(lParam);
				m_rtOffset.bottom -= m_ptClick.y - HIWORD(lParam);
				m_ptClick.x = LOWORD(lParam);
				m_ptClick.y = HIWORD(lParam);
			}
		}break;
		case WM_LBUTTONUP:
		{
			m_bDrag = false;
			m_rtOffset.left = 0;
			m_rtOffset.bottom = 0;
		}break;
		case WM_MOUSEWHEEL:
		{
			m_fWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f;
		}break;

	}
	return -1;
}

bool HCamera::Init()
{
	PostInit();
	return true;
}

bool HCamera::Frame()
{
	Vector3 vUp = { 0,1,0 };
	m_matView = Matrix::CreateLookAt(m_vCameraPos, m_vCameraTarget, vUp);
	UpdateVector();

	return true;
}

bool HCamera::CreateOrthographic(float width, float height, float fN, float fF)
{
	m_matProject = Matrix::CreateOrthographic(width, height, fN, fF);

	return true;
}

bool HCamera::DrawFrustum(ID3D11DeviceContext * pd3dContext, Matrix * pmatView, Matrix * pmatProj)
{
	return true;
}

bool HCamera::CreateViewMatrix(Vector3 p, Vector3 t, Vector3 u)
{
	m_vCameraPos = p;
	m_vCameraTarget = t;
	m_fDistance = (m_vCameraPos - m_vCameraTarget).Length();
	m_matView = Matrix::CreateLookAt(p, t, u);
	

	Matrix mInvView;
	mInvView = m_matView.Invert();
	Vector3* pZBasis = (Vector3*)&m_matView._31;

	m_vDirValue.y = atan2f(pZBasis->x, pZBasis->z);
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_vDirValue.x = -atan2f(pZBasis->y, fLen);

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

}

void HCamera::FrontMovement(float fDir)
{
	Vector3 vFrontMove = m_vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vFrontMove;
}

void HCamera::RightMovement(float fDir)
{
	Vector3 vRightMove = m_vRight * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vRightMove;
}

void HCamera::UpMovement(float fDir)
{
	Vector3 vUpMove = m_vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vCameraPos += vUpMove;
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
	m_fWheelDelta = 0;
}

HCamera::~HCamera()
{
}

