#include "Sample.h"
#define NUM_OBJECTS 10

LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);

	return -1;
}

Matrix* TD3DXMatrixShadow(Matrix *pout,
	Vector4 *plight,
	Vector4 *pplane)
{
	Vector4 Nplane;
	FLOAT dot;
	Nplane.Normalize();
	//D3DXPlaneNormalize(&Nplane, pplane);
	dot = Nplane.Dot(*plight);
	pout->m[0][0] = dot - Nplane.x * plight->x;
	pout->m[0][1] = -Nplane.x * plight->y;
	pout->m[0][2] = -Nplane.x * plight->z;
	pout->m[0][3] = -Nplane.x * plight->w;
	pout->m[1][0] = -Nplane.y * plight->x;
	pout->m[1][1] = dot - Nplane.y * plight->y;
	pout->m[1][2] = -Nplane.y * plight->z;
	pout->m[1][3] = -Nplane.y * plight->w;
	pout->m[2][0] = -Nplane.z * plight->x;
	pout->m[2][1] = -Nplane.z * plight->y;
	pout->m[2][2] = dot - Nplane.z * plight->z;
	pout->m[2][3] = -Nplane.z * plight->w;
	pout->m[3][0] = -Nplane.w * plight->x;
	pout->m[3][1] = -Nplane.w * plight->y;
	pout->m[3][2] = -Nplane.w * plight->z;
	pout->m[3][3] = dot - Nplane.w * plight->w;
	return pout;
}

Matrix Sample::CreateMatrixShadow(
	Vector4* pPlane,
	Vector4* pLight)
{
	Matrix mat;
	Vector4 plane, light;
	pPlane->Normalize();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane.Dot(light));
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}



void Sample::DrawObject(Matrix* pView, Matrix* pProj)
{
	for (int iBox = 0; iBox < NUM_OBJECTS; iBox++)
	{
		m_pObject[iBox].m_matWorld._42 =
			m_Map.GetHeightMap(m_pObject[iBox].m_matWorld._41,
				m_pObject[iBox].m_matWorld._43);

		m_BoxShape.SetMatrix(&m_pObject[iBox].m_matWorld,
			pView,
			pProj);
		// OBB와 프로스텀 박스의 제외처리( 걸쳐 있어도 TRUE가 됨. )
		if (m_pMainCamera->m_Frustum.CheckOBBInPlane(&m_pObject[iBox].m_hBox))
		{
			m_BoxShape.Render(g_pImmediateContext);
		}
	}
}

bool Sample::Init()
{
	HRESULT hr;

	if (!m_Sky.Create(g_pd3dDevice, L"VS.txt", L"SkyObject.txt", L""))
	{
		return false;
	}

	m_pConstantLightBuffer[0].Attach(HBASIS_CORE_LIB::CreateConstantBuffer
							(g_pd3dDevice, &m_cbLight1, 1, sizeof(LIGHT_CONSTANT_BUFFER)));
	m_pConstantLightBuffer[1].Attach(HBASIS_CORE_LIB::CreateConstantBuffer
							(g_pd3dDevice, &m_cbLight2, 1, sizeof(LIGHT_CONSTANT_BUFFER2)));
	m_pConstantLightBuffer[2].Attach(HBASIS_CORE_LIB::CreateConstantBuffer
							(g_pd3dDevice, &m_cbLight3, 1, sizeof(LIGHT_CONSTANT_BUFFER3)));

	/////////////////////////////////////////////////////////////////////////////
	// 유저 생성
	/////////////////////////////////////////////////////////////////////////////
	if (!m_UserShape.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}


	/////////////////////////////////////////////////////////////////////////////
	// 높이맵 생성
	/////////////////////////////////////////////////////////////////////////////
	if (!m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext, L"../../Image/data/map/HEIGHT_CASTLE.bmp"))
	{
		return false;
	}

	HMapDesc desc;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.iNumRows = m_Map.m_iNumRows;
	desc.fCellDistance = 1.0f;
	desc.fScaleHeight = 10.0f;
	desc.szTextFile = L"../../Image/data/map/castle.jpg";
	desc.szVS = L"VS.txt";
	desc.szPS = L"PS.txt";
	m_Map.CreateMap(g_pd3dDevice, desc);

	m_vDirValue = { 0,0,0,0 };

	/////////////////////////////////////////////////////////////////////////////
	// 미니맵 생성
	/////////////////////////////////////////////////////////////////////////////
	m_Minimap.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg");

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(100, 100, 0);
	matRotation = Matrix::CreateRotationX(HBASIS_PI * 0.5f);
	m_matPlaneWorld = matScale * matRotation;

	if (!m_BoxShape.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}

	// 쿼드트리 공간분할
	m_QuadTree.Build(m_Map);

	if (!m_PlaneShape.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}

	
	// 카메라 바꿔치는 부분.
	m_ModelCamera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjectionMatrix(1, 1000, HBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();
	// 프러스텀 생성
	m_ModelCamera.CreateFrustum(g_pd3dDevice, g_pImmediateContext);
	m_pMainCamera = &m_ModelCamera;

	m_TopCamera.CreateViewMatrix({ 0,30,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_TopCamera.CreateOrthographic(desc.iNumCols, desc.iNumRows, 1.0f, 1000);
	m_TopCamera.Init();


	return true;
}

bool Sample::Frame()
{

	// 메테리얼
	m_cbLight1.g_cAmbientMaterial[0] = Vector4{ 0.1f, 0.1f, 0.1f, 1 };
	m_cbLight1.g_cDiffuseMaterial[0] = Vector4{ 1, 1, 1, 1 };
	m_cbLight1.g_cSpecularMaterial[0] = Vector4{ 1, 1, 1, 1 };
	m_cbLight1.g_cEmissionMaterial[0] = Vector4{ 0, 0, 0, 1 };
	// 라이트컬러
	m_cbLight1.g_cAmbientLightColor[0] = Vector4{ 0.3f, 0.3f, 0.3f, 1 };
	m_cbLight1.g_cDiffuseLightColor[0] = Vector4{ 1, 0, 0, 1.0f };
	m_cbLight1.g_cSpecularLightColor[0] = Vector4{ 1, 1, 1, 1 };

	m_cbLight1.g_cAmbientLightColor[1] = Vector4{ 0.3f, 0.3f, 0.3f, 1 };
	m_cbLight1.g_cDiffuseLightColor[1] = Vector4{ 0, 1, 0, 1.0f };
	m_cbLight1.g_cSpecularLightColor[1] = Vector4{ 1, 1, 1, 1 };

	m_cbLight1.g_cAmbientLightColor[2] = Vector4{ 0.3f, 0.3f, 0.3f, 1 };
	m_cbLight1.g_cDiffuseLightColor[2] = Vector4{ 0, 0, 1, 1.0f };
	m_cbLight1.g_cSpecularLightColor[2] = Vector4{ 1, 1, 1, 1 };

	float fTime = 32.0f;
	m_cbLight2.g_vLightPos[0] = Vector4{ 0, 30, 0, 200 }; // 범위
	m_cbLight2.g_vLightPos[1] = Vector4{ 50, 0, 0, 200 }; // 범위
	m_cbLight2.g_vLightPos[2] = Vector4{ 0, 0, 50, 200 }; // 범위

	// 방향 위에서 아래로
	m_cbLight2.g_vLightDir[0] = -m_cbLight2.g_vLightPos[0];
	m_cbLight2.g_vLightDir[0].w = 1.0f;
	m_cbLight2.g_vLightDir[0].Normalize();
	m_cbLight2.g_vLightDir[1] = -m_cbLight2.g_vLightPos[1];
	m_cbLight2.g_vLightDir[1].w = 1.0f;
	m_cbLight2.g_vLightDir[1].Normalize();
	m_cbLight2.g_vLightDir[2] = -m_cbLight2.g_vLightPos[2];
	m_cbLight2.g_vLightDir[2].w = 1.0f;
	m_cbLight2.g_vLightDir[2].Normalize();

	for (int iLight = 0; iLight < g_iNumLight; iLight++)
	{
		Matrix matInvWorld;
		m_cbLight2.g_matInvWorld[iLight] = Matrix::Identity;
		m_cbLight2.g_vEyeDir[iLight].x = m_pMainCamera->m_vLook.x;
		m_cbLight2.g_vEyeDir[iLight].y = m_pMainCamera->m_vLook.y;
		m_cbLight2.g_vEyeDir[iLight].z = m_pMainCamera->m_vLook.z;
		m_cbLight2.g_vEyeDir[iLight].w = 10.0f; // 강도
	}

	m_cbLight3.g_vSpotInfo[0].x = cosf(30.0f * (HBASIS_PI / 180.0f)); // 내부 콘 각도 범위
	m_cbLight3.g_vSpotInfo[0].y = cosf(30.0f * (HBASIS_PI / 180.0f)); // 외부 콘 각도 범위
	m_cbLight3.g_vSpotInfo[0].z = 4; // 내부 콘과 외부 콘의 휘도
	m_cbLight3.g_vSpotInfo[0].w = 100;  // 라이트 범위

	m_cbLight3.g_vSpotInfo[1].x = cosf(90.0f * (HBASIS_PI / 180.0f)); // 내부 콘 각도 범위
	m_cbLight3.g_vSpotInfo[1].y = cosf(120.0f * (HBASIS_PI / 180.0f)); // 외부 콘 각도 범위
	m_cbLight3.g_vSpotInfo[1].z = 4; // 내부 콘과 외부 콘의 휘도
	m_cbLight3.g_vSpotInfo[1].w = 100;  // 라이트 범위

	m_cbLight3.g_vSpotInfo[2].x = cosf(90.0f * (HBASIS_PI / 180.0f)); // 내부 콘 각도 범위
	m_cbLight3.g_vSpotInfo[2].y = cosf(120.0f * (HBASIS_PI / 180.0f)); // 외부 콘 각도 범위
	m_cbLight3.g_vSpotInfo[2].z = 4; // 내부 콘과 외부 콘의 휘도
	m_cbLight3.g_vSpotInfo[2].w = 100;  // 라이트 범위

	// ConstantBuffer 정보 업데이트
	g_pImmediateContext->UpdateSubresource(m_pConstantLightBuffer[0].Get(), 0, NULL, &m_cbLight1, 0, 0);
	g_pImmediateContext->UpdateSubresource(m_pConstantLightBuffer[1].Get(), 0, NULL, &m_cbLight2, 0, 0);
	g_pImmediateContext->UpdateSubresource(m_pConstantLightBuffer[2].Get(), 0, NULL, &m_cbLight3, 0, 0);

	ID3D11Buffer* pBuffers[3];
	pBuffers[0] = m_pConstantLightBuffer[0].Get();
	pBuffers[1] = m_pConstantLightBuffer[1].Get();
	pBuffers[2] = m_pConstantLightBuffer[2].Get();
	// 픽셀쉐이더에 버퍼 전달
	g_pImmediateContext->PSSetConstantBuffers(1, 3, pBuffers);

	/////////////////////////////////////////////////////////////////////////////////////////////////

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_UserShape.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_UserShape.RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_UserShape.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_UserShape.UpMovement(-1.0f);
	}

	m_UserShape.Frame();
	m_UserShape.m_vPos.y = m_Map.GetHeightMap(m_UserShape.m_matWorld._41, m_UserShape.m_matWorld._43);

	m_pMainCamera->m_vCameraTarget = m_UserShape.m_vPos;
	m_pMainCamera->FrameFrustum(g_pImmediateContext);

	m_UserShape.m_matRotation = m_pMainCamera->m_matWorld;


	return true;
}

bool Sample::Render()
{

	m_Sky.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_Sky.Render(g_pImmediateContext);

	// Culling
	//std::vector<DWORD> visibleIB;
	//visibleIB.resize(m_Map.m_IndexList.size());
	//m_Map.m_iNumFaces = 0;
	//for (int iFace = 0; iFace < m_Map.m_IndexList.size() / 3; iFace++)
	//{
	//	int a = m_Map.m_IndexList[iFace * 3 + 0];
	//	int b = m_Map.m_IndexList[iFace * 3 + 1];
	//	int c = m_Map.m_IndexList[iFace * 3 + 2];
	//	//visibleIB.push_back(a);
	//	//visibleIB.push_back(b);
	//	//visibleIB.push_back(c);
	//	//continue;

	//	Vector3 v[3];
	//	v[0] = m_Map.m_VertexList[a].p;
	//	v[1] = m_Map.m_VertexList[b].p;
	//	v[2] = m_Map.m_VertexList[c].p;
	//	HModelViewCamera* pCamera = (HModelViewCamera*)m_pMainCamera;
	//	for (int iV = 0; iV < 3; iV++)
	//	{
	//		BOOL bVisiable = pCamera->m_Frustum.ClassifyPoint(v[iV]);
	//		if (bVisiable)
	//		{
	//			/*visibleIB.push_back(a);
	//			visibleIB.push_back(b);
	//			visibleIB.push_back(c);*/
	//			visibleIB[m_Map.m_iNumFaces * 3 + 0] = a;
	//			visibleIB[m_Map.m_iNumFaces * 3 + 1] = b;
	//			visibleIB[m_Map.m_iNumFaces * 3 + 2] = c;
	//			m_Map.m_iNumFaces++;
	//			break;
	//		}
	//	}
	//}

	//if (visibleIB.size() != 0)
	//{
	//	m_Map.m_iNumFaces = visibleIB.size() / 3;
	//	m_pd3dContext->UpdateSubresource(
	//		m_Map.m_pIndexBuffer, 0, NULL, &visibleIB.at(0), 0, 0);
	//}
	//else
	//{
	//	m_Map.m_iNumFaces = 0;
	//}

	if (m_Minimap.Begin(g_pImmediateContext))
	{
		m_Map.SetMatrix(NULL, &m_TopCamera.m_matView, &m_TopCamera.m_matProject);
		m_Map.Render(g_pImmediateContext);

		Matrix matWorld;
		matWorld._41 = m_TopCamera.m_vCameraPos.x;
		matWorld._42 = m_TopCamera.m_vCameraPos.y;
		matWorld._43 = m_TopCamera.m_vCameraPos.z;

		m_UserShape.SetMatrix(NULL, &m_TopCamera.m_matView, &m_TopCamera.m_matProject);
		m_UserShape.Render(g_pImmediateContext);

		m_BoxShape.SetMatrix(NULL, &m_TopCamera.m_matView, &m_TopCamera.m_matProject);
		m_BoxShape.Render(g_pImmediateContext);

		// DrawObject(&m_TopCamera.m_matView, &m_TopCamera.m_matProject);

		// 미니맵 중앙에 배치
		/*Vector3 vPos = m_BoxShape.m_vPos - m_BoxShape.m_vLook;
		vPos.y = 30.0f;
		m_TopCamera.CreateViewMatrix(vPos, m_BoxShape.m_vPos);
		m_pMainCamera->DrawFrustum(m_pd3dContext, &m_TopCamera.m_matView, &m_TopCamera.m_matProject);*/

		m_Minimap.End(g_pImmediateContext);
	}


	m_Map.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_Map.m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
	m_Map.m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
	m_Map.m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
	m_Map.Render(g_pImmediateContext);

	//Matrix matShadow;
	//Vector4 PLANE = Vector4(0, 1, 0, -0.1f);
	//Vector3 vLightDir = Vector3(-10, 10, 0);
	//// Vector4 LIGHT = Vector4(-10, 10, 0, 1);
	//// matShadow = CreateMatrixShadow(&PLANE, &LIGHT);
	//matShadow = Matrix::CreateShadow(vLightDir, PLANE);

	//matShadow = m_matBoxWorld * matShadow;
	//m_BoxShape.SetMatrix(&matShadow, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	//m_BoxShape.Render(m_pd3dContext);

	m_UserShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_UserShape.Render(g_pImmediateContext);

	m_Minimap.SetMatrix(NULL, NULL, NULL);
	m_Minimap.Render(g_pImmediateContext);

	//DrawQuadLine(m_QuadTree.m_pRootNode);
	// DrawObject(&m_pMainCamera->m_matView,
	//  	&m_pMainCamera->m_matProject);

	return true;
}

bool Sample::PostRender()
{
	HCore::PostRender();
	return true;
}

bool Sample::Release()
{
	m_Sky.Release();
	SAFE_DELETE_ARRAY(m_pObject);
	m_Minimap.Release();
	m_Map.Release();
	m_UserShape.Release();
	m_BoxShape.Release();
	m_PlaneShape.Release();

	return true;
}


void HBoxUser::FrontMovement(float fDir)
{
	Vector3 vFrontMove = m_vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vFrontMove;
}

void HBoxUser::RightMovement(float fDir)
{
	Vector3 vRightMove = m_vRight * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vRightMove;
}

void HBoxUser::UpMovement(float fDir)
{
	Vector3 vUpMove = m_vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vUpMove;
}

void HBoxUser::FrontBase(float fDir)
{
	Vector3 vLook = { 0,0,1 };
	Vector3 vMove = vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vMove;
	m_vTarget += m_vLook * m_fSpeed;
}

void HBoxUser::RightBase(float fDir)
{
	Vector3 vSide = { 1,0,0 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vMove;
	m_vTarget += m_vLook * m_fSpeed;
}

void HBoxUser::UpBase(float fDir)
{
	Vector3 vUp = { 0,1,0 };
	Vector3 vMove = vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vMove;
	m_vTarget += m_vLook * m_fSpeed;
}


