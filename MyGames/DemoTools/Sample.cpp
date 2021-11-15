#include "Sample.h"

int Sample::m_iTileCount = 257;
float Sample::m_fCellCount = 10.0f;
float Sample::m_fScale = 10.0f;

ID3D11Texture2D* Sample::LoadTexturetMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* pFilename)
{
	HRESULT hr;
	ID3D11Resource* pTexture;
	size_t maxsize = 0;

	// 불러온 Texture를 CPU가 읽어들일 수 있도록 D3D11_USAGE_STAGING,
	// D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ 로 할당.
	if (FAILED(hr = CreateWICTextureFromFileEx(pDevice,
		pFilename,
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		WIC_LOADER_DEFAULT,
		&pTexture, nullptr)))
	{

		if (FAILED(hr = CreateDDSTextureFromFileEx(pDevice,
			pFilename,
			maxsize,
			D3D11_USAGE_STAGING,
			0,
			D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
			0,
			WIC_LOADER_DEFAULT,
			&pTexture, nullptr)))
		{
			return false;
		}

	}

	ID3D11Texture2D* pTexture2D = NULL;
	// QueryInterface
	// 개체가 특정의 구성 요소 개체 모델 (COM) 인터페이스를 지원 하고 있을지 어떨지를 판별한다. 
	// 인터페이스가 지원 되고 있는 경우, 시스템은 개체의 참조 카운트를 늘려, 
	// 애플리케이션은, 그 인터페이스를 곧바로 사용할 수 있다.
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	return pTexture2D;
}

void Sample::SaveMapData()
{
	ofstream fout;
	fout.open("save.txt");
	m_Save.iTileCount = m_iTileCount;
	m_Save.fCellCount = m_fCellCount;
	m_Save.fScale = m_fScale;
	m_Save.fileName = Filename;
	m_Save.iListSize = m_Map.m_VertexList.size();


	int len = 256;
	char cTemp[256];
	WideCharToMultiByte(CP_ACP, 0, m_Save.fileName, len, cTemp, len, NULL, NULL);

	fout << cTemp << endl << m_Save.iTileCount << endl
		<< m_Save.fCellCount << endl << m_Save.fScale << endl
		<< m_Save.iListSize << endl;

	m_Save.m_SaveVertexList.resize(m_Map.m_VertexList.size());

	for (int i = 0; i < m_Map.m_VertexList.size(); i++)
	{
		m_Save.m_SaveVertexList[i].y = m_Map.m_VertexList[i].p.y;
		fout << m_Save.m_SaveVertexList[i].y << endl;
	}

	fout.close();

	isave = 0;

}

void Sample::LoadMapData(string filename)
{

	ifstream fin;
	char input[256];

	fin.open(filename);
	if (fin.fail())
	{
		return;
	}

	fin >> input
		>> m_Save.iTileCount
		>> m_Save.fCellCount
		>> m_Save.fScale
		>> m_Save.iListSize;

	string name = input;
	m_Save.fileName = StringToTCHAR(name);

	m_Save.m_SaveVertexList.resize(m_Save.iListSize);
	for (int i = 0; i < m_Save.m_SaveVertexList.size(); i++)
	{
		fin >> m_Save.m_SaveVertexList[i].y;
	}

}

void Sample::SaveTextureFile(ID3D11Texture2D* texture, T_STR name)
{
	HRESULT hr;
	ComPtr<ID3D11Texture2D> SaveTex;
	if (texture == nullptr)
	{
		return;
	}
	else
	{
		SaveTex = texture;
	}
	hr = DirectX::SaveDDSTextureToFile(g_pImmediateContext, SaveTex.Get(), (name + L".dds").c_str());
	if (FAILED(hr))
	{
		return;
	}
}

LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);

	return -1;
}

bool Sample::GetIntersection(HNode* pNode)
{
	// face list
	for (int face = 0; face < pNode->m_IndexList.size() / 3; face++)
	{
		v0 = m_Map.m_VertexList[pNode->m_IndexList[face * 3 + 0]].p;
		v1 = m_Map.m_VertexList[pNode->m_IndexList[face * 3 + 1]].p;
		v2 = m_Map.m_VertexList[pNode->m_IndexList[face * 3 + 2]].p;

		vEnd = m_Picking.m_Ray.vOrigin + m_Picking.m_Ray.vDir * 10000.0f;
		vNormal = (v1 - v0).Cross(v2 - v0);
		vNormal.Normalize();

		// 방법 1) 외적을 사용하는 방법
		if (m_Picking.GetIntersection(m_Picking.m_Ray.vOrigin, vEnd, vNormal, v0, v1, v2))
		{
			if (m_Picking.PointInPolygon(m_Picking.m_vInterSection, vNormal, v0, v1, v2))
			{
				list[0] = v0;
				list[1] = v1;
				list[2] = v2;

				return true;
			}
		}

		// 방법 2) UV 매개변수를 사용한 교점과 교점 포함 테스트를
		//         동시에 처리 하는 방법(평면 노말 필요X).
		//if (m_Picking.IntersectTriangle(vPickRayOrigin, vPickRayDir, v0, v1, v2, t, u, v))
		//{
		//	list[0] = v0;
		//	list[1] = v1;
		//	list[2] = v2;
		//	return true;
		//}
	}

	return false;
}

TCHAR* Sample::StringToTCHAR(string& s)
{
	string tstr;
	const char* all = s.c_str();
	int len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

#pragma region Init
bool Sample::Init()
{
	HRESULT hr;
	Filename = StringToTCHAR(m_FileName);

	if (!m_ShapeBox.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L""))
	{
		return false;
	}

	m_Camera.CreateViewMatrix({ 0,300,-100 }, { 0,0,0 });

	if (m_bObjInitState)
	{
		m_pObj.Init();

		m_bObjInitState = false;
	}
	m_pObj.m_pMainCamera = m_pMainCamera;

#pragma region UserCreate
	/////////////////////////////////////////////////////////////////////////////
	// 유저 생성
	/////////////////////////////////////////////////////////////////////////////
	if (!m_UserShape.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}
#pragma endregion

#pragma region HeightMap
	/////////////////////////////////////////////////////////////////////////////
	// 높이맵 생성
	/////////////////////////////////////////////////////////////////////////////
	//if (!m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext, L"../../Image/data/map/129.jpg"))
	//{
	//	return false;
	//}
#pragma endregion

#pragma region MapCreate
	HMapDesc desc;
	if (m_LoadData)
	{
		LoadMapData("save.txt");
		desc.iNumCols = m_Save.iTileCount;
		desc.iNumRows = m_Save.iTileCount;
		desc.fCellDistance = m_Save.fCellCount;
		desc.fScaleHeight = m_Save.fScale;
		desc.szTextFile = m_Save.fileName;
		desc.szVS = L"../../data/Shader/ToolBaseVS.txt";
		desc.szPS = L"../../data/Shader/ToolBasePS.txt";
		//m_Map.SetLoadHeight(m_Save.m_SaveVertexList);
		m_Map.CreateMap(g_pd3dDevice, desc);

		for (int i = 0; i < m_Save.m_SaveVertexList.size(); i++)
		{
			m_Map.m_VertexList[i].p.y = m_Save.m_SaveVertexList[i].y;
		}

		m_LoadTexture = LoadTexturetMap(g_pd3dDevice, g_pImmediateContext, L"SaveAlphaTex.dds");
	}
	else
	{
		//m_iTileCount = 513;
		desc.iNumCols = m_iTileCount;
		desc.iNumRows = m_iTileCount;
		desc.fCellDistance = m_fCellCount;
		desc.fScaleHeight = m_fScale;
		desc.szTextFile = Filename;
		desc.szVS = L"../../data/Shader/ToolBaseVS.txt";
		desc.szPS = L"../../data/Shader/ToolBasePS.txt";
		m_Map.CreateMap(g_pd3dDevice, desc);
	}
#pragma endregion

#pragma region MinimapCreate
	if (!m_Minimap.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", Filename))
	{
		return false;
	}
#pragma endregion

#pragma region TextureMapCreate
	if (!m_TextureMap.Create(&m_Map, g_pd3dDevice, g_pImmediateContext, L"HeightMapVS.txt", L"HeightMapPS.txt", L""))
	{
		return false;
	}
#pragma endregion

#pragma region AlphaZeroMap Create
	if (!m_AlphaZeroTexture.Create(&m_Map, g_pd3dDevice, g_pImmediateContext, L"HeightMapVS.txt", L"HeightMapPS.txt", Filename))
	{
		return false;
	}
#pragma endregion
#pragma region SplattingTexture CreateShaderResourceView
	m_pTextureSRV[1] = CreateShaderResourceView(g_pd3dDevice, L"../../Image/data/map/woodfloor.bmp");
	m_pTextureSRV[2] = CreateShaderResourceView(g_pd3dDevice, L"../../Image/data/map/seafloor.bmp");
	m_pTextureSRV[3] = CreateShaderResourceView(g_pd3dDevice, L"../../Image/data/map/stone_wall.bmp");
	m_pTextureSRV[4] = CreateShaderResourceView(g_pd3dDevice, L"../../Image/data/map/land.bmp");
#pragma endregion

#pragma region QuadTree Create
	// 쿼드트리 공간분할
	m_QuadTree.Build(&m_Map);
#pragma endregion

#pragma region Camera Init
	// 카메라 바꿔치는 부분.
	m_ModelCamera.CreateViewMatrix({ 0,30,-30 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjectionMatrix(1, 10000, HBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();

	m_TopCamera.CreateViewMatrix({ 0,30,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	int x = desc.iNumCols * m_fCellCount;
	int y = desc.iNumRows * m_fCellCount;
	m_TopCamera.CreateOrthographic(x, y, 1, 10000);
	m_TopCamera.Init();
#pragma endregion

	HCore::m_bFrameRun = true;

	// 프러스텀 생성
	//m_ModelCamera.CreateFrustum(g_pd3dDevice, g_pImmediateContext);
	//m_pMainCamera = &m_ModelCamera;

	if (m_LoadData)
	{
		m_AlphaZeroTexture.PickRenderTextureData(&m_Map, m_LoadTexture,
			g_pImmediateContext, pick, m_iSplattingNum);
		g_pImmediateContext->CopyResource(m_AlphaZeroTexture.pTexture, m_LoadTexture);
		m_pTextureSRV[0] = m_AlphaZeroTexture.m_pSRV;
		//m_LoadData = false;
	}
	else
	{
		m_AlphaZeroTexture.PickRenderTextureData(&m_Map, m_AlphaZeroTexture.pStaging,
			g_pImmediateContext, pick, m_iSplattingNum);
		g_pImmediateContext->CopyResource(m_AlphaZeroTexture.pTexture, m_AlphaZeroTexture.pStaging);
		m_pTextureSRV[0] = m_AlphaZeroTexture.m_pSRV;
	}

	isave = 1;

	InitWork();

	return true;
}
#pragma endregion

#pragma region Frame
bool Sample::Frame()
{
	if (g_Input.GetKey(VK_HOME) == KEY_PUSH)
	{
		m_SelectData[0].fRadius = 100 + rand() % 300;
		m_SelectData[0].iIndex = rand() % 4; // z
		float x = (rand() % m_iTextureSizeX) - m_iTextureSizeX / 2.0f;
		float y = (rand() % m_iTextureSizeX) - m_iTextureSizeX / 2.0f;
		m_SelectData[0].vPickPos = Vector3(x, y, 0);
		m_SelectData[0].fTexHeight = 1024.0f;
		m_SelectData[0].fTexWidth = 1024.0f;
		g_pImmediateContext->UpdateSubresource(m_pSelectBuffer.Get(),
			0, NULL, &m_SelectData, 0, 0);

		ID3D11ShaderResourceView* arraySRV[3] =
		{
			m_Map.m_pTexture->m_pTextureSRV,
			m_pAlphaTextureSRVCopy.Get(),
			m_pSelectBufferSRV.Get(),
		};

		RunComputeShader(g_pImmediateContext, m_pCS.Get(),
			arraySRV, m_pAlphaTextureUAV.GetAddressOf(),
			3,
			m_iTextureSizeX / 32, m_iTextureSizeY / 32, 1);

		g_pImmediateContext->CopyResource(m_pAlphaTextureCopy.Get(), m_pAlphaTexture.Get());
	}

	m_bSelect = false;

	if (m_SaveTexture)
	{
		SaveMapData();
		//const string name = "save.txt";
		//LoadMapData(name);
		SaveTextureFile(m_AlphaZeroTexture.pTexture, L"../../save/SaveTex");
		m_SaveTexture = false;
	}

#pragma region Mouse Picking
	if (m_bMoveObj || m_bFieldUpdateState || m_bSplattingState)
	{
		PickingSelect = KEY_HOLD;
	}
	else
	{
		PickingSelect = KEY_PUSH;
	}
	if (g_Input.GetKey(VK_LBUTTON) == PickingSelect && g_Input.GetKey(VK_SHIFT))
	{
		m_bSelect = true;
		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(g_hWnd, &cursor);
		Matrix matProj = m_pMainCamera->m_matProject;

		Vector3 vec;
		// 화면 -> 투영 -> 뷰
		vec.x = (((2.0f*cursor.x) / g_rtClient.right) - 1) / matProj._11;
		vec.y = -(((2.0f*cursor.y) / g_rtClient.bottom) - 1) / matProj._22;
		vec.z = 1.0f;

		Matrix matInvView = m_pMainCamera->m_matView;
		matInvView = matInvView.Invert();

		// 방법 1) 직접 정점 변환
		vPickRayDir.x = vec.x * matInvView._11 + vec.y * matInvView._21 + vec.z * matInvView._31;
		vPickRayDir.y = vec.x * matInvView._12 + vec.y * matInvView._22 + vec.z * matInvView._32;
		vPickRayDir.z = vec.x * matInvView._13 + vec.y * matInvView._23 + vec.z * matInvView._33;
		vPickRayDir.Normalize();

		vPickRayOrigin.x = matInvView._41;
		vPickRayOrigin.y = matInvView._42;
		vPickRayOrigin.z = matInvView._43;

		// 방법 2) 함수 사용 정점 변환
		//vPickRayOrigin = Vector3{ 0.0f, 0.0f, 0.0f };
		//vPickRayDir = vec;
		//vPickRayOrigin = Vector3::Transform(vPickRayOrigin, matInvView);
		//vPickRayDir = Vector3::TransformNormal(vPickRayDir, matInvView);
		//vPickRayDir.Normalize();

		m_Picking.m_Ray.vDir = vPickRayDir;
		m_Picking.m_Ray.vOrigin = vPickRayOrigin;

	}
#pragma endregion

#pragma region ObjectControl

	if (m_bObjFrameState && m_bSelect)
	{
		m_pObj.Frame();

		for (int i = 0; i < m_QuadTree.m_leafList.size(); i++)
		{
			HNode* pNode = m_QuadTree.m_leafList[i];

			if (m_Picking.IntersectBox(&pNode->m_hBox, &m_Picking.m_Ray))
			{
				m_SelectNode.push_back(pNode);
			}
		}

		float fMaxDist = 99999;
		bool Update = false;
		for (int select = 0; select < m_SelectNode.size(); select++)
		{
			HNode* pNode = m_SelectNode[select];
			if (GetIntersection(pNode))
			{
				float fDistance = (m_Picking.m_Ray.vOrigin - m_Picking.m_vInterSection).Length();
				if (fMaxDist > fDistance)
				{
					Update = true;
					pick = m_Picking.m_vInterSection;
					fMaxDist = fDistance;
				}
			}

		}

		if (Update)
		{
			if (m_bCreateObj)
			{
				Matrix matWorld;
				HModel model;
				H_SPHERE sphere;

				matWorld._41 = m_Picking.m_vInterSection.x;
				matWorld._42 = m_Picking.m_vInterSection.y;
				matWorld._43 = m_Picking.m_vInterSection.z;

				model.Init();

				sphere.fRadius = 80.0f;
				sphere.vCenter = m_Picking.m_vInterSection;

				collision.sphere = sphere;
				collision.mat = matWorld;
				collision.pModel = model;

				m_ModelMatrixList.push_back(matWorld);
				m_ColisionList.push_back(collision);
			}

			if (m_bObjDelete)
			{
				for (int i = 0; i < m_ColisionList.size(); i++)
				{
					if (m_Select.IntersectRayToSphere(&m_ColisionList[i].sphere, &m_Picking.m_Ray))
					{
						m_ColisionList.erase(m_ColisionList.begin() + i);
						m_ModelMatrixList.erase(m_ModelMatrixList.begin() + i);
					}
				}
			}

			if (m_bMoveObj)
			{

				for (int i = 0; i < m_ColisionList.size(); i++)
				{
					if (m_Select.IntersectRayToSphere(&m_ColisionList[i].sphere, &m_Picking.m_Ray))
					{
						m_ColisionList[i].sphere.fRadius = 2000.0f;
						m_ColisionList[i].sphere.vCenter = m_Picking.m_vInterSection;
						m_ModelMatrixList[i]._41 = m_Picking.m_vInterSection.x;
						m_ModelMatrixList[i]._43 = m_Picking.m_vInterSection.z;
						m_ColisionList[i].mat._41 = m_ModelMatrixList[i]._41;
						m_ColisionList[i].mat._43 = m_ModelMatrixList[i]._43;
						SelectNum = i;
					}

				}

			}

			if (m_bScale)
			{
				for (int i = 0; i < m_ColisionList.size(); i++)
				{
					if (m_Select.IntersectRayToSphere(&m_ColisionList[i].sphere, &m_Picking.m_Ray))
					{
						Matrix trans;
						trans._41 = m_ModelMatrixList[i]._41;
						trans._43 = m_ModelMatrixList[i]._43;

						m_ModelMatrixList[i] = m_ModelMatrixList[i].CreateScale(m_ScaleCount);
						m_ModelMatrixList[i]._41 = trans._41;
						m_ModelMatrixList[i]._43 = trans._43;

						m_ColisionList[i].mat = m_ColisionList[i].mat.CreateScale(m_ScaleCount);
						m_ColisionList[i].mat._41 = trans._41;
						m_ColisionList[i].mat._43 = trans._43;
					}
				}
			}

			if (m_bRotation)
			{
				for (int i = 0; i < m_ColisionList.size(); i++)
				{
					if (m_Select.IntersectRayToSphere(&m_ColisionList[i].sphere, &m_Picking.m_Ray))
					{
						Matrix trans;
						Matrix rotate;

						rotate = Matrix::CreateRotationY(m_RotationCount * (HBASIS_PI / 180.0f));

						trans._41 = m_ModelMatrixList[i]._41;
						trans._43 = m_ModelMatrixList[i]._43;

						m_ModelMatrixList[i] = m_ModelMatrixList[i] * rotate;
						m_ModelMatrixList[i]._41 = trans._41;
						m_ModelMatrixList[i]._43 = trans._43;

						m_ColisionList[i].mat = m_ColisionList[i].mat * rotate;
						m_ColisionList[i].mat._41 = trans._41;
						m_ColisionList[i].mat._43 = trans._43;

					}
				}
			}

		}

	}
	if (!m_bSelect && m_bMoveObj)
	{
		m_ColisionList[SelectNum].sphere.fRadius = 80.0f;
		for (int i = 0; i < m_ColisionList.size(); i++)
		{
			m_ColisionList[i].mat._41 = m_ModelMatrixList[i]._41;
			m_ColisionList[i].mat._43 = m_ModelMatrixList[i]._43;
		}

	}

#pragma endregion

	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		//높이맵 갱신
		m_TextureMap.Frame(&m_Map, g_pImmediateContext);
	}

#pragma region PickTextureSplatting
	if (m_bSelect && m_bSplattingState)
	{
		bool Update = false;
		if (!m_bFieldUpdateState)
		{
			for (int i = 0; i < m_QuadTree.m_leafList.size(); i++)
			{
				HNode* pNode = m_QuadTree.m_leafList[i];

				if (m_Picking.IntersectBox(&pNode->m_hBox, &m_Picking.m_Ray))
				{
					m_SelectNode.push_back(pNode);
				}
			}

			float fMaxDist = 99999;
			for (int select = 0; select < m_SelectNode.size(); select++)
			{
				HNode* pNode = m_SelectNode[select];
				if (GetIntersection(pNode))
				{
					float fDistance = (m_Picking.m_Ray.vOrigin - m_Picking.m_vInterSection).Length();
					if (fMaxDist > fDistance)
					{
						Update = true;
						pick = m_Picking.m_vInterSection;
						fMaxDist = fDistance;
					}
				}

			}
		}


		m_SelectData[0].fRadius = m_fRadius;
		switch (m_iSplattingNum)
		{
		case 1: m_SelectData[0].iIndex = 0; break;
		case 2: m_SelectData[0].iIndex = 1; break;
		case 3: m_SelectData[0].iIndex = 2; break;
		case 4: m_SelectData[0].iIndex = 3; break;

		default:
			break;
		}

		m_SelectData[0].fTexHeight = 1024.0f;
		m_SelectData[0].fTexWidth = 1024.0f;

		float fMapWidth = m_Map.m_iNumCellCols * m_Map.m_fCellDistance;
		float fWidthRatio = m_iTextureSizeX / fMapWidth;
		Vector2 vCenter = Vector2((pick.x + (fMapWidth / 2.0f)) * fWidthRatio, (-(pick.z - (fMapWidth / 2.0f))) * fWidthRatio);



		float x = vCenter.x;
		float z = vCenter.y;


		m_SelectData[0].vPickPos = Vector3(x, z, 0);
		g_pImmediateContext->UpdateSubresource(m_pSelectBuffer.Get(),
			0, NULL, &m_SelectData, 0, 0);

		ID3D11ShaderResourceView* arraySRV[3] =
		{
			m_Map.m_pTexture->m_pTextureSRV,
			m_pAlphaTextureSRVCopy.Get(),
			m_pSelectBufferSRV.Get(),
		};

		RunComputeShader(g_pImmediateContext, m_pCS.Get(),
			arraySRV, m_pAlphaTextureUAV.GetAddressOf(),
			3,
			m_iTextureSizeX / 32, m_iTextureSizeY / 32, 1);

		g_pImmediateContext->CopyResource(m_pAlphaTextureCopy.Get(), m_pAlphaTexture.Get());

		// 주석 제거시 상단 중앙 RPG맵 활성화
		//if (m_LoadData)
		//{
		//	m_AlphaZeroTexture.PickRenderTextureData(&m_Map, m_LoadTexture,
		//		g_pImmediateContext, pick, m_iSplattingNum);
		//	g_pImmediateContext->CopyResource(m_AlphaZeroTexture.pTexture, m_LoadTexture);
		//	m_pTextureSRV[0] = m_AlphaZeroTexture.m_pSRV;
		//}
		//else
		//{
		//	m_AlphaZeroTexture.PickRenderTextureData(&m_Map, m_AlphaZeroTexture.pStaging,
		//		g_pImmediateContext, pick, m_iSplattingNum);
		//	g_pImmediateContext->CopyResource(m_AlphaZeroTexture.pTexture, m_AlphaZeroTexture.pStaging);
		//	m_pTextureSRV[0] = m_AlphaZeroTexture.m_pSRV;
		//}
	}
#pragma endregion

#pragma region UserInput
	/*
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
		}*/
#pragma endregion


	m_UserShape.Frame();

	m_pMainCamera->m_vCameraTarget = m_UserShape.m_vPos;
	//m_pMainCamera->FrameFrustum(g_pImmediateContext);

	m_UserShape.m_matRotation = m_pMainCamera->m_matWorld;

	return true;
}
#pragma endregion

#pragma region Render
bool Sample::Render()
{
	// TODO : Alpha 0.5f
	// ApplyBS(g_pImmediateContext, HDxState::g_pAlphaBlend);
	m_AlphaZeroTexture.SetRadius(m_fRadius);

#pragma region ModelRender
	for (auto data : m_ModelMatrixList)
	{
		m_pObj.m_pFbxObj->SetMatrix(&data, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
		m_pObj.Render(g_pImmediateContext);
	}
#pragma endregion

#pragma region Rasterize
	if (g_Input.GetKey('1') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('2') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('3') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.CullMode = D3D11_CULL_BACK;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('4') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.CullMode = D3D11_CULL_FRONT;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(20.0f);

	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(-20.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(-20.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(20.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(20.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(-20.0f);
	}
#pragma endregion

#pragma region Field Update
	m_SelectNode.clear();

	if (m_bSelect && m_bFieldUpdateState)
	{
		for (int i = 0; i < m_QuadTree.m_leafList.size(); i++)
		{
			HNode* pNode = m_QuadTree.m_leafList[i];

			if (m_Picking.IntersectBox(&pNode->m_hBox, &m_Picking.m_Ray))
			{
				m_SelectNode.push_back(pNode);
			}
		}

		float fMaxDist = 99999;
		bool Update = false;
		for (int select = 0; select < m_SelectNode.size(); select++)
		{
			HNode* pNode = m_SelectNode[select];
			if (GetIntersection(pNode))
			{
				float fDistance = (m_Picking.m_Ray.vOrigin - m_Picking.m_vInterSection).Length();
				if (fMaxDist > fDistance)
				{
					Update = true;
					pick = m_Picking.m_vInterSection;
					fMaxDist = fDistance;
				}
			}

		}


		m_ControlNode.clear();
		H_SPHERE pickSphere;
		pickSphere.vCenter = pick;
		pickSphere.fRadius = m_fRadius;

		for (int i = 0; i < m_QuadTree.m_leafList.size(); i++)
		{
			HNode* pNode = m_QuadTree.m_leafList[i];
			if (HCollision::SphereToSphere(pNode->m_Sphere, pickSphere))
			{
				m_ControlNode.push_back(pNode);
			}
		}

		if (m_bIncreaseGround)
		{
			float transY = 0.0f;
			float beforeY = 0.0f;

			for (auto node : m_ControlNode)
			{
				for (int v = 0; v < node->m_IndexList.size(); v++)
				{
					int iID = node->m_IndexList[v];
					float fDist = (m_Map.m_VertexList[iID].p - pick).Length();
					if (fDist < m_fRadius)
					{
						if (m_Map.m_VertexList[iID].p.y < 255)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = v.y + m_fSpeed - sinf((fDist / m_fRadius));
							beforeY = m_Map.m_VertexList[iID].p.y;
							if (beforeY > transY)
							{
								transY = beforeY;
							}
						}
						beforePos.y = pick.y;
					}
				}
				// 실시간 노말 계산
				//m_Map.CalcPerVertexNormalsFastLookUp();
				for (int i = 0; i < m_ColisionList.size(); i++)
				{
					if (m_Select.IntersectRayToSphere(&m_ColisionList[i].sphere, &m_Picking.m_Ray))
					{
						m_ColisionList[i].sphere.fRadius = m_fRadius;
						m_ModelMatrixList[i]._42 = transY;
						m_ColisionList[i].mat._42 = transY;
					}

				}
			}
		}

		if (m_bDecreaseGround)
		{
			float transY = 0.0f;
			float beforeY = 0.0f;

			for (auto node : m_ControlNode)
			{
				for (int v = 0; v < node->m_IndexList.size(); v++)
				{
					int iID = node->m_IndexList[v];
					float fDist = (m_Map.m_VertexList[iID].p - pick).Length();
					if (fDist < m_fRadius)
					{
						if (m_Map.m_VertexList[iID].p.y > -255)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = v.y - m_fSpeed - sinf((fDist / m_fRadius));
							beforePos.y = pick.y;
							beforeY = m_Map.m_VertexList[iID].p.y;
							if (beforeY < transY)
							{
								transY = beforeY;
							}
						}
					}
				}
				// 실시간 노말 계산
				//m_Map.CalcPerVertexNormalsFastLookUp();
				for (int i = 0; i < m_ColisionList.size(); i++)
				{
					if (m_Select.IntersectRayToSphere(&m_ColisionList[i].sphere, &m_Picking.m_Ray))
					{
						m_ColisionList[i].sphere.fRadius = m_fRadius;
						m_ModelMatrixList[i]._42 = transY;
						m_ColisionList[i].mat._42 = transY;
					}

				}
			}
		}


		if (m_bOriginGround)
		{
			float transY = 0.0f;
			float beforeY = 0.0f;

			for (auto node : m_ControlNode)
			{
				for (int v = 0; v < node->m_IndexList.size(); v++)
				{
					int iID = node->m_IndexList[v];
					float fDist = (m_Map.m_VertexList[iID].p - pick).Length();
					beforePos.y = 0.0f;
					if (fDist < m_fRadius)
					{
						if (m_Map.m_VertexList[iID].p.y >= 0)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = beforePos.y - m_fSpeed - sinf((fDist / m_fRadius));
							beforeY = m_Map.m_VertexList[iID].p.y;
							if (beforeY < transY)
							{
								transY = beforeY;
							}
						}
						if (m_Map.m_VertexList[iID].p.y <= 0)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = beforePos.y + m_fSpeed - sinf((fDist / m_fRadius));
							beforeY = m_Map.m_VertexList[iID].p.y;
							if (beforeY > transY)
							{
								transY = beforeY;
							}
						}

					}
				}
				// 실시간 노말 계산
				//m_Map.CalcPerVertexNormalsFastLookUp();
				for (int i = 0; i < m_ColisionList.size(); i++)
				{
					if (m_Select.IntersectRayToSphere(&m_ColisionList[i].sphere, &m_Picking.m_Ray))
					{
						m_ColisionList[i].sphere.fRadius = m_fRadius;
						m_ModelMatrixList[i]._42 = transY;
						m_ColisionList[i].mat._42 = transY;
					}

				}
			}
		}

		if (m_bFlatGrond)
		{
			float transY = 0.0f;
			float beforeY = 0.0f;

			for (auto node : m_ControlNode)
			{
				for (int v = 0; v < node->m_IndexList.size(); v++)
				{
					int iID = node->m_IndexList[v];
					float fDist = (m_Map.m_VertexList[iID].p - pick).Length();
					if (fDist < m_fRadius)
					{
						if (m_Map.m_VertexList[iID].p.y >= beforePos.y || m_Map.m_VertexList[iID].p.y <= beforePos.y)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = beforePos.y + m_fSpeed - sinf((fDist / m_fRadius));
							transY = m_Map.m_VertexList[iID].p.y;
							//if (beforeY > transY)
							//{
							//	transY = beforeY;
							//}
						}
					}
				}
				// 실시간 노말 계산
				//m_Map.CalcPerVertexNormalsFastLookUp();
				for (int i = 0; i < m_ColisionList.size(); i++)
				{
					if (m_Select.IntersectRayToSphere(&m_ColisionList[i].sphere, &m_Picking.m_Ray))
					{
						m_ColisionList[i].sphere.fRadius = m_fRadius;
						m_ModelMatrixList[i]._42 = transY;
						m_ColisionList[i].mat._42 = transY;
					}

				}
			}
		}

		g_pImmediateContext->UpdateSubresource(m_Map.m_pVertexBuffer, 0, NULL, &m_Map.m_VertexList.at(0), 0, 0);
	}
#pragma endregion

#pragma region MapRender
	m_Map.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_Map.m_cbData.vColor[0] = m_pMainCamera->m_vLook.x*1.5f;
	m_Map.m_cbData.vColor[1] = m_pMainCamera->m_vLook.y*1.5f;
	m_Map.m_cbData.vColor[2] = m_pMainCamera->m_vLook.z*1.5f;
	for (auto node : m_QuadTree.m_leafList)
	{
		//g_pImmediateContext->PSSetShaderResources(1, 1, &m_pTextureSRV[0]);
		//g_pImmediateContext->PSSetShaderResources(2, 4, &m_pTextureSRV[1]);
		g_pImmediateContext->PSSetShaderResources(1, 1, &m_pSplatting[0]->m_pTextureSRV);
		g_pImmediateContext->PSSetShaderResources(2, 1, &m_pSplatting[1]->m_pTextureSRV);
		g_pImmediateContext->PSSetShaderResources(3, 1, &m_pSplatting[2]->m_pTextureSRV);
		g_pImmediateContext->PSSetShaderResources(4, 1, &m_pSplatting[3]->m_pTextureSRV);
		g_pImmediateContext->PSSetShaderResources(5, 1, m_pAlphaTextureSRVCopy.GetAddressOf());
		m_QuadTree.Draw(g_pImmediateContext, node);
	}
#pragma endregion

#pragma region MiniMapRender
	m_Minimap.SetMatrix(NULL, NULL, NULL);
	if (m_Minimap.Begin(g_pImmediateContext))
	{
		m_Map.SetMatrix(NULL, &m_TopCamera.m_matView, &m_TopCamera.m_matProject);
		for (auto node : m_QuadTree.m_leafList)
		{
			g_pImmediateContext->PSSetShaderResources(1, 1, &m_pSplatting[0]->m_pTextureSRV);
			g_pImmediateContext->PSSetShaderResources(2, 1, &m_pSplatting[1]->m_pTextureSRV);
			g_pImmediateContext->PSSetShaderResources(3, 1, &m_pSplatting[2]->m_pTextureSRV);
			g_pImmediateContext->PSSetShaderResources(4, 1, &m_pSplatting[3]->m_pTextureSRV);
			g_pImmediateContext->PSSetShaderResources(5, 1, m_pAlphaTextureSRVCopy.GetAddressOf());
			//g_pImmediateContext->PSSetShaderResources(1, 1, &m_pTextureSRV[0]);
			//g_pImmediateContext->PSSetShaderResources(2, 4, &m_pTextureSRV[1]);
			//m_QuadTree.Draw(g_pImmediateContext, node);
		}
		m_Map.Render(g_pImmediateContext);
		m_Minimap.End(g_pImmediateContext);
	}
#pragma endregion	

	Matrix mat;
	mat._41 = 1.5f;

	Matrix matOrtho;
	matOrtho._41 = 0.75f;
	m_AlphaZeroTexture.SetMatrix(NULL, NULL, &matOrtho);
	m_AlphaZeroTexture.Render(g_pImmediateContext);

	m_UserShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_UserShape.Render(g_pImmediateContext);

	m_Minimap.SetMatrix(NULL, NULL, NULL);
	m_Minimap.Render(g_pImmediateContext);

	m_TextureMap.SetMatrix(NULL, NULL, &mat);
	m_TextureMap.Render(g_pImmediateContext);

	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_LineShape.Draw(g_pImmediateContext, vPickRayOrigin, vPickRayOrigin + vPickRayDir * 100.0f);
	m_LineShape.Draw(g_pImmediateContext, list[0], list[1]);
	m_LineShape.Draw(g_pImmediateContext, list[1], list[2]);
	m_LineShape.Draw(g_pImmediateContext, list[2], list[0]);

	return true;
}
#pragma endregion

#pragma region PostRender
bool Sample::PostRender()
{
	HCore::PostRender();
	return true;
}
#pragma endregion

#pragma region Release
bool Sample::Release()
{
	if (m_pTextureSRV[1])
	{
		m_pTextureSRV[1]->Release();
		m_pTextureSRV[2]->Release();
		m_pTextureSRV[3]->Release();
		m_pTextureSRV[4]->Release();
	}

	m_pObj.Release();
	m_QuadTree.Release();
	m_Minimap.Release();
	m_Map.Release();
	m_UserShape.Release();

	return true;
}
#pragma endregion

#pragma region BoxMovement
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
#pragma endregion
