#include "HShape.h"

void HShape::CompilerCheck(ID3DBlob* ErrorMsg)
{
	C_STR szMsg = (char*)ErrorMsg->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}

bool HShape::Create(ID3D11Device * pDevice, T_STR vs, T_STR ps, T_STR texture)
{
	m_pd3dDevice = pDevice;

	CreateVertexData();
	CreateVertexBuffer();
	CreateConstantBuffer();
	CreateIndexData();
	CreateIndexBuffer();
	LoadShader(vs, ps);
	CreateInputLayOut();
	LoadTexture(texture);
	
	return true;
}

bool HShape::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	/*UINT ByteWidth;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;*/
	bd.ByteWidth = sizeof(P_VERTEX) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	// Vertex 버퍼로 바인딩
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool HShape::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(P_VERTEX) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	// 인덱스 버퍼로 바인딩
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool HShape::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(HDataCB);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// Constant 버퍼로 바인딩
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cbData;
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_ConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool HShape::CreateVertexData()
{
	return true;
}

bool HShape::CreateIndexData()
{
	return true;
}

bool HShape::CreateInputLayOut()
{
	// LayOut
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = m_pd3dDevice->CreateInputLayout(layout, iNumElement, pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(), &m_pInputLayout);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool HShape::Update(ID3D11DeviceContext* pd3dContext)
{

	D3D11_MAPPED_SUBRESOURCE ms;
	HRESULT hr = pd3dContext->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	if (SUCCEEDED(hr))
	{
		HDataCB* pData = (HDataCB*)ms.pData;
		pData->matWorld = m_matWorld.Transpose();
		pData->matView = m_matView.Transpose();
		pData->matProject = m_matProject.Transpose();

		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer) * 0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		// map 사용 후 unmap 쓰레드. mutex와 같음
		pd3dContext->Unmap(m_ConstantBuffer, 0);
	}

	return true;
}

bool HShape::LoadShader(T_STR vs, T_STR ps)
{
	// 쉐이더
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsg;

	// Vertex 쉐이더
	HRESULT hr = D3DCompileFromFile(vs.c_str(), NULL, NULL, m_szVertexShader.c_str(), "vs_5_0", 0, 0, &pVSObj, &pErrorMsg);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsg);
		return false;
	}
	hr = m_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL,
		&m_pVertexShader);

	// Pixel 쉐이더
	hr = D3DCompileFromFile(ps.c_str(), NULL, NULL, m_szPixelShader.c_str(), "ps_5_0", 0, 0, &pPSObj, &pErrorMsg);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsg);
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL,
		&m_pPixelShader);

	return true;
}

bool HShape::LoadTexture(T_STR texture)
{
	// 텍스쳐 로드
	ID3D11Resource* textrueResource;
	HRESULT hr = DirectX::CreateWICTextureFromFile(m_pd3dDevice, texture.c_str(), NULL, &m_pTextureSRV);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool HShape::SetMatrix(HMatrix * pWorld, HMatrix * pView, HMatrix * pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
	}
	if (pView != nullptr)
	{
		m_matView = *pView;
	}
	if (pProj != nullptr)
	{
		m_matProject = *pProj;
	}

	return true;
}

bool HShape::Init()
{
	m_matWorld.Identity();
	m_matView.Identity();
	m_matProject.Identity();

	return true;
}

bool HShape::Frame()
{
	return true;
}

bool HShape::Render(ID3D11DeviceContext* pContext)
{
	Update(pContext);
	UINT iStride = sizeof(P_VERTEX);
	UINT iOffset = 0;
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetInputLayout(m_pInputLayout);

	pContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	pContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	pContext->VSSetShader(m_pVertexShader, NULL, 0);
	pContext->PSSetShader(m_pPixelShader, NULL, 0);
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	pContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	// Vertex Draw
	// m_pd3dContext->Draw(m_VertexList.size(), 0);

	// Index Draw
	pContext->DrawIndexed(m_IndexList.size(), 0, 0);

	return true;
}

bool HShape::Release()
{
	m_ConstantBuffer->Release();
	m_pTextureSRV->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();

	return true;
}

HShape::HShape()
{
	m_szVertexShader = "VS";
	m_szPixelShader = "PS";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

HShape::~HShape()
{
}

bool HShapeBox::CreateVertexData()
{
	m_VertexList.resize(24);

	m_VertexList =
	{
		// 앞면
	   { HVector3(-1.0f, 1.0f, -0.5f),
		 HVector3(0,0,0),
		 HVector4(1,0,0,1),
		 HVector2(0,0)
	   },

	   { HVector3(1.0f, 1.0f, -0.5f),
		 HVector3(0,0,0),
		 HVector4(0,1,0,1),
		 HVector2(1,0) },

	   { HVector3(1.0f, -1.0f, -0.5f),
		 HVector3(0,0,0),
		 HVector4(0,0,1,1),
		 HVector2(0,1) },

	   { HVector3(-1.0f, -1.0f, -0.5f),
		 HVector3(0,0,0),
		 HVector4(1,1,1,1),
		 HVector2(1,1) },

		// 뒷면
	   { HVector3(1.0f, 1.0f, 0.5f),
		 HVector3(0,0,0),
		 HVector4(1,0,0,1),
		 HVector2(0,0)
	   },

	   { HVector3(-1.0f, 1.0f, 0.5f),
		 HVector3(0,0,0),
		 HVector4(0,1,0,1),
		 HVector2(1,0) },

	   { HVector3(-1.0f, -1.0f, 0.5f),
		 HVector3(0,0,0),
		 HVector4(0,0,1,1),
		 HVector2(0,1) },

	   { HVector3(1.0f, -1.0f, 0.5f),
		 HVector3(0,0,0),
		 HVector4(1,1,1,1),
		 HVector2(1,1) },

		// 위
		{ HVector3(-1.0f, 1.0f, 0.5f),
		  HVector3(0,0,0),
		  HVector4(1,0,0,1),
		  HVector2(0,0)
		},

		{ HVector3(1.0f, 1.0f, 0.5f),
		  HVector3(0,0,0),
		  HVector4(0,1,0,1),
		  HVector2(1,0) },

		{ HVector3(1.0f, 1.0f, -0.5f),
		  HVector3(0,0,0),
		  HVector4(0,0,1,1),
		  HVector2(0,1) },

		{ HVector3(-1.0f, 1.0f, -0.5f),
		  HVector3(0,0,0),
		  HVector4(1,1,1,1),
		  HVector2(1,1) },

		// 아래
		{ HVector3(1.0f, -1.0f, 0.5f),
		  HVector3(0,0,0),
		  HVector4(1,0,0,1),
		  HVector2(0,0)
		},

		{ HVector3(-1.0f, -1.0f, 0.5f),
		  HVector3(0,0,0),
		  HVector4(0,1,0,1),
		  HVector2(1,0) },

		{ HVector3(-1.0f, -1.0f, -0.5f),
		  HVector3(0,0,0),
		  HVector4(0,0,1,1),
		  HVector2(0,1) },

		{ HVector3(1.0f, -1.0f, -0.5f),
		  HVector3(0,0,0),
		  HVector4(1,1,1,1),
		  HVector2(1,1) },

		// 왼쪽
		{ HVector3(-1.0f, 1.0f, 0.5f),
		  HVector3(0,0,0),
		  HVector4(1,0,0,1),
		  HVector2(0,0)
		},

		{ HVector3(-1.0f, 1.0f, -0.5f),
		  HVector3(0,0,0),
		  HVector4(0,1,0,1),
		  HVector2(1,0) },

		{ HVector3(-1.0f, -1.0f, -0.5f),
		  HVector3(0,0,0),
		  HVector4(0,0,1,1),
		  HVector2(0,1) },

		{ HVector3(-1.0f, -1.0f, 0.5f),
		  HVector3(0,0,0),
		  HVector4(1,1,1,1),
		  HVector2(1,1) },

		// 오른쪽
		{ HVector3(1.0f, 1.0f, -0.5f),
		  HVector3(0,0,0),
		  HVector4(1,0,0,1),
		  HVector2(0,0)
		},

		{ HVector3(1.0f, 1.0f, 0.5f),
		  HVector3(0,0,0),
		  HVector4(0,1,0,1),
		  HVector2(1,0) },

		{ HVector3(1.0f, -1.0f, 0.5f),
		  HVector3(0,0,0),
		  HVector4(0,0,1,1),
		  HVector2(0,1) },

		{ HVector3(1.0f, -1.0f, -0.5f),
		  HVector3(0,0,0),
		  HVector4(1,1,1,1),
		  HVector2(1,1) },

	};

	return true;
}

bool HShapeBox::CreateIndexData()
{
	m_IndexList.resize(36);

	m_IndexList =
	{
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23,
	};

	return true;
}

HShapeBox::HShapeBox()
{
}

HShapeBox::~HShapeBox()
{
}

bool HShapePlane::CreateVertexData()
{
	m_VertexList.resize(4);
	m_VertexList =
	{
	   { HVector3(-1.0f, 1.0f, -0.5f),
		 HVector3(0,0,0),
		 HVector4(1,0,0,1),
		 HVector2(0,0)
	   },

	   { HVector3(1.0f, 1.0f, -0.5f),
		 HVector3(0,0,0),
		 HVector4(0,1,0,1),
		 HVector2(1,0) },

	   { HVector3(1.0f, -1.0f, -0.5f),
		 HVector3(0,0,0),
		 HVector4(0,0,1,1),
		 HVector2(0,1) },

	   { HVector3(-1.0f, -1.0f, -0.5f),
		 HVector3(0,0,0),
		 HVector4(1,1,1,1),
		 HVector2(1,1) },
	};

	return true;
}

bool HShapePlane::CreateIndexData()
{
	m_IndexList.resize(6);
	m_IndexList =
	{
		0, 1, 2,
		0, 2, 3,
	};

	return true;
}

HShapePlane::HShapePlane()
{
}

HShapePlane::~HShapePlane()
{
}

bool HShapeLine::Draw(ID3D11DeviceContext * pContext, HVector3 p, HVector3 e, HVector4 c)
{
	m_VertexList = 
	{
	   { p,
		 HVector3(0.0f,0.0f,-1.0f),
		 c,
		 HVector2(0,0)
	   },

	   { e,
		 HVector3(0.0f,0.0f,-1.0f),
		 c,
		 HVector2(1,0) },
	};
	pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);

	return HShape::Render(pContext);
}

bool HShapeLine::CreateVertexData()
{
	m_VertexList.resize(2);
	m_VertexList =
	{
	   { HVector3(0.0f, 0.0f, 0.0f),
		 HVector3(0.0f, 0.0f, -1.0f),
		 HVector4(1.0f ,0.0f ,0.0f ,1.0f),
		 HVector2(0,0)
	   },

	   { HVector3(100.0f, 0.0f, 0.0f),
		 HVector3(0.0f, 0.0f, -1.0f),
		 HVector4(1.0f ,0.0f ,0.0f ,1.0f),
		 HVector2(1.0f, 0) },
	};

	return true;
}

bool HShapeLine::CreateIndexData()
{
	m_IndexList.resize(2);
	m_IndexList =
	{
		0, 1
	};
	return true;
}

HShapeLine::HShapeLine()
{
	m_szPixelShader = "PSLine";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

HShapeLine::~HShapeLine()
{
}
