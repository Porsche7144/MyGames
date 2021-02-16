#include "HDebugCamera.h"
#include "HInput.h"

void HDebugCamera::Update(Vector4 data)
{
	Matrix matRotation;
	matRotation = Matrix::CreateFromYawPitchRoll(data.y, data.x, data.z);

	m_vCameraPos += m_vLook * m_fWheelDelta;
	// HBASIS_CORE_LIB::OutputDebug("%10.4f\n", m_fWheelDelta);
	matRotation._41 = m_vCameraPos.x;
	matRotation._42 = m_vCameraPos.y;
	matRotation._43 = m_vCameraPos.z;

	m_matView = matRotation.Invert();
	m_fWheelDelta = 0;
}

bool HDebugCamera::Init()
{
	HCamera::Init();
	m_ptPrePosition = g_Input.GetPos();
	return true;
}

bool HDebugCamera::Frame()
{
	POINT point;
	GetCursorPos(&point);
	HWND hWnd = WindowFromPoint(point);
	ScreenToClient(g_hWnd, &point);

	if (hWnd == g_hWnd && g_Input.GetKey(VK_LBUTTON))
	{
		float fAngleX = (point.x - m_ptPrePosition.x);
		float fAngleY = (point.y - m_ptPrePosition.y);
		m_vDirValue.y += XMConvertToRadians(fAngleX / 2.0f);
		m_vDirValue.x += XMConvertToRadians(fAngleY / 2.0f);
	}

	m_ptPrePosition = point;

	Update(m_vDirValue);
	UpdateVector();
	
	return true;
}

HDebugCamera::HDebugCamera()
{
	m_vDirValue.x = 0.0f;
	m_vDirValue.y = 0.0f;
	m_vDirValue.z = 0.0f;
	m_vDirValue.w = 0.0f;
}

HDebugCamera::~HDebugCamera()
{
}
