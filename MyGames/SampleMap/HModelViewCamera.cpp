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
		m_WorldArcBall.OnBegin(iMouseX, iMouseY);
	}break;
	case WM_MOUSEMOVE:
	{
		m_WorldArcBall.OnMove(iMouseX, iMouseY);
		m_ViewArcBall.OnMove(iMouseX, iMouseY);
	}break;
	case WM_LBUTTONUP:
	{
		m_WorldArcBall.OnEnd();
	}break;
	case WM_RBUTTONUP:
	{
		m_ViewArcBall.OnEnd();
	}break;
	case WM_RBUTTONDOWN:
	{
		m_ViewArcBall.OnBegin(iMouseX, iMouseY);
	}break;
	case WM_MOUSEWHEEL:
	{
		m_fWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
	}break;

	}
	return -1;
}

Matrix HArcBall::GetRotationMatrix()
{
	return Matrix::CreateFromQuaternion(m_qNow);
}

Quaternion HArcBall::QuatFromPoints(Vector3 v0, Vector3 v1)
{
	// 사원수(Quaternion)의 벡터 외적 내적 계산.
	// 각도에 상관 없이 두개의 벡터만 있다면 이동값이 계산되기 때문에
	// 직관적이며 편리하다.
	Vector3 vCross;
	float fDot = v0.Dot(v1);
	vCross = v0.Cross(v1);

	return Quaternion(vCross.x, vCross.y, vCross.z, fDot);
}

Vector3 HArcBall::ScreenToVector(float px, float py)
{
	// 구면방정식
	// 화면이 -1 ~ +1 일때,
	// z*z + y*y + x*x = 1
	// 따라서 z = sqrt(1 - (x*x + y*y))
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

void HArcBall::OnBegin(int x, int y)
{
	// m_qDown 이전프레임 값
	// m_qNow 현재프레임 값
	m_bDrag = true;
	m_vDownPt = ScreenToVector(x, y);
	m_qDown = m_qNow;
}

void HArcBall::OnMove(int x, int y)
{
	if (m_bDrag)
	{
		m_vCurrentPt = ScreenToVector(x, y);
		m_qNow = m_qDown * QuatFromPoints(m_vDownPt, m_vCurrentPt);
	}
}

void HArcBall::OnEnd()
{
	m_bDrag = false;
}

void HModelViewCamera::Update(Vector4 d)
{
	

}

bool HModelViewCamera::CreateFrustum(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dContext)
{
	m_Frustum.Create(pd3dDevice);
	return true;
}

bool HModelViewCamera::DrawFrustum(ID3D11DeviceContext* pd3dContext, Matrix* pmatView, Matrix*  pmatProj)
{
	m_Frustum.m_FrustumObj.SetMatrix(NULL, pmatView, pmatProj);
	m_Frustum.m_FrustumObj.Render(pd3dContext);

	return true;
}

bool HModelViewCamera::PostInit()
{
	Matrix matInvView = m_matView.Invert();
	m_ViewArcBall.m_qNow = Quaternion::CreateFromRotationMatrix(matInvView);
	m_ViewArcBall.m_qNow.Normalize();

	return true;
}

bool HModelViewCamera::Frame()
{

	// View
	m_fDistance += m_fWheelDelta;
	Matrix matCameraRotation;
	matCameraRotation = m_ViewArcBall.GetRotationMatrix();
	Vector3 vLocalUp = { 0,1,0 };
	Vector3 vLocalLook = { 0,0,1 };
	vLocalUp = Vector3::Transform(vLocalUp, matCameraRotation);
	vLocalLook = Vector3::Transform(vLocalLook, matCameraRotation);
	vLocalUp.Normalize();
	vLocalLook.Normalize();

	m_vCameraPos = m_vCameraTarget - vLocalLook * m_fDistance;
	CreateViewMatrix(m_vCameraPos, m_vCameraTarget);

	// World
	Matrix mlnvVIew = m_matView.Invert();
	// 이동행렬은 제외.
	mlnvVIew._41 = 0.0f;
	mlnvVIew._42 = 0.0f;
	mlnvVIew._43 = 0.0f;

	Matrix mModelRotInv = m_mModelLastRot.Invert();

	Matrix mModelRot = m_WorldArcBall.GetRotationMatrix();

	// 뷰행렬에서 회전처리를 하기 위해 m_matView를 곱함.
	// 이전 회전행렬값을 빼기 위해 mModelRotInv를 곱함.
	// 누적된 회전행렬값 mModelRot
	// 쉐이더 파이프라인에서 뷰행렬이 계산되기 때문에 m_matView의 역행렬인 mlnvVIew로 계산.
	m_matWorld = m_matWorld * m_matView * mModelRotInv * mModelRot * mlnvVIew;
	m_matWorld._41 = 0.0f;
	m_matWorld._42 = 0.0f;
	m_matWorld._43 = 0.0f;
	m_mModelLastRot = mModelRot;

	UpdateVector();

	m_fWheelDelta = 0;
	return true;
}

bool HModelViewCamera::FrameFrustum(ID3D11DeviceContext * pd3dContext)
{
	Matrix matInvViewProj = m_matView * m_matProject;
	matInvViewProj = matInvViewProj.Invert();
	for (int iVertex = 0; iVertex < 24; iVertex++)
	{
		Vector3& v = m_Frustum.m_VertexList[iVertex].p;
		m_Frustum.m_FrustumObj.m_VertexList[iVertex].p = Vector3::Transform(v, matInvViewProj);// *matInvViewProj;
	}
	pd3dContext->UpdateSubresource(m_Frustum.m_FrustumObj.m_pVertexBuffer, 0, NULL,
								  &m_Frustum.m_FrustumObj.m_VertexList.at(0), 0, 0);

	m_Frustum.Frame();


	return true;
}

void HModelViewCamera::UpdateVector()
{
	HCamera::UpdateVector();
}

HModelViewCamera::HModelViewCamera()
{
}

HModelViewCamera::~HModelViewCamera()
{
	m_Frustum.m_FrustumObj.Release();
}
