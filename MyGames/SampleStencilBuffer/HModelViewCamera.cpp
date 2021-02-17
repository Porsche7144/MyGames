#include "HModelViewCamera.h"

int HModelViewCamera::WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	int iMouseX = LOWORD(lParam);
	int iMouseY = HIWORD(lParam);

	HRESULT hitTest;
	switch (message)
	{

	case WM_LBUTTONDOWN:
	{
		m_bDrag = true;
		OnBegin(iMouseX, iMouseY);
	}break;
	case WM_MOUSEMOVE:
	{
		if (m_bDrag)
		{
			OnMove(iMouseX, iMouseY);
		}
	}break;
	case WM_LBUTTONUP:
	{
		m_bDrag = false;
		OnEnd();
	}break;
	case WM_MOUSEWHEEL:
	{
		m_fWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f;
	}break;

	}
	return -1;
}

Matrix HModelViewCamera::GetRotationMatrix()
{
	return Matrix::CreateFromQuaternion(m_qDown);
}

Quaternion HModelViewCamera::QuatFromPoints(Vector3 v0, Vector3 v1)
{
	// �����(Quaternion)�� ���� ���� ���� ���.
	// ������ ��� ���� �ΰ��� ���͸� �ִٸ� �̵����� ���Ǳ� ������
	// �������̸� ���ϴ�.
	Vector3 vCross;
	float fDot = v0.Dot(v1);
	vCross = v0.Cross(v1);

	return Quaternion(vCross.x, vCross.y, vCross.z, fDot);
}

Vector3 HModelViewCamera::ScreenToVector(float px, float py)
{
	// ���������
	// ȭ���� -1 ~ +1 �϶�,
	// z*z + y*y + x*x = 1
	// ���� z = sqrt(1 - (x*x + y*y))
	float x = -((px / (float)g_rtClient.right) * 2.0f - 1.0f);
	float y = (px / (float)g_rtClient.right) * 2.0f - 1.0f;
	float z = 0.0f;
	float fDist = x * x + y * y;
	if (fDist > 1.0f)
	{
		float fScale = (1.0f / sqrtf(fDist));
		x *= fScale;
		y *= fScale;
	}
	else
	{
		z = sqrtf(1.0f - fDist);
	}

	return Vector3(x, y, z);
}

void HModelViewCamera::OnBegin(int x, int y)
{
	m_vDownPt = ScreenToVector(x, y);
	m_qDown = m_qNow;
}

void HModelViewCamera::OnMove(int x, int y)
{
	m_vCurrentPt = ScreenToVector(x, y);
	m_qDown = m_qNow * QuatFromPoints(m_vDownPt, m_vCurrentPt);
}

void HModelViewCamera::OnEnd()
{
}

void HModelViewCamera::Update(Vector4 d)
{
	Matrix mlnvVIew = m_matView.Invert();
	// �̵������ ����.
	mlnvVIew._41 = 0;
	mlnvVIew._42 = 0;
	mlnvVIew._43 = 0;

	Matrix mModelRotInv = m_mModelLastRot.Invert();

	Matrix mModelRot = GetRotationMatrix();

	// ����Ŀ��� ȸ��ó���� �ϱ� ������ m_matView�� ����.
	// ���� ȸ����İ��� ���� ���� mModelRotInv�� ����.
	// ������ ȸ����İ� mModelRot
	// ���̴� ���������ο��� ������� ���Ǳ� ������ m_matView�� ������� mlnvVIew�� ���.
	m_matWorld = m_matWorld * m_matView * mModelRotInv * mModelRot * mlnvVIew;
	m_matWorld._41 = 0.0f;
	m_matWorld._42 = 0.0f;
	m_matWorld._43 = 0.0f;

	m_mModelLastRot = mModelRot;

}

bool HModelViewCamera::Frame()
{
	Matrix mlnvVIew = m_matView.Invert();
	// �̵������ ����.
	mlnvVIew._41 = 0.0f;
	mlnvVIew._42 = 0.0f;
	mlnvVIew._43 = 0.0f;

	Matrix mModelRotInv = m_mModelLastRot.Invert();

	Matrix mModelRot = GetRotationMatrix();

	// ����Ŀ��� ȸ��ó���� �ϱ� ���� m_matView�� ����.
	// ���� ȸ����İ��� ���� ���� mModelRotInv�� ����.
	// ������ ȸ����İ� mModelRot
	// ���̴� ���������ο��� ������� ���Ǳ� ������ m_matView�� ������� mlnvVIew�� ���.
	m_matWorld = m_matWorld * m_matView * mModelRotInv * mModelRot * mlnvVIew;
	m_matWorld._41 = 0.0f;
	m_matWorld._42 = 0.0f;
	m_matWorld._43 = 0.0f;

	m_mModelLastRot = mModelRot;
	UpdateVector();

	return true;
}

HModelViewCamera::HModelViewCamera()
{
	m_qDown = Quaternion::Identity;
	m_qNow = Quaternion::Identity;
}

HModelViewCamera::~HModelViewCamera()
{
}
