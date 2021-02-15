#include "HDebugCamera.h"

void HDebugCamera::Update(Vector4 data)
{
	Matrix matRotation;
	matRotation = Matrix::CreateFromYawPitchRoll(data.y, data.x, data.z);

	m_vCameraPos += m_vLook * data.w;
	matRotation._41 = m_vCameraPos.x;
	matRotation._42 = m_vCameraPos.y;
	matRotation._43 = m_vCameraPos.z;

	m_matView = matRotation.Invert();
}

bool HDebugCamera::Frame()
{
	m_vDirValue.y += m_rtOffset.left * g_fSecondPerFrame * 0.01f;
	m_vDirValue.x += m_rtOffset.bottom * g_fSecondPerFrame * 0.01f;
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
