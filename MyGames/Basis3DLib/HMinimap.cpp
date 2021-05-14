#include "HMinimap.h"

bool HMinimap::CreateVertexData()
{
	m_VertexList.resize(4);
	m_VertexList =
	{
	   { PNCT_VERTEX(
		 Vector3(-1.0f, 1.0f, 0.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(0.0f,0.0f))
	   },

	   { PNCT_VERTEX(
		 Vector3(-0.5f, 1.0f, 0.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(1.0f,0.0f)) },

	   { PNCT_VERTEX(
		 Vector3(-0.5f, 0.5f, 0.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(1.0f, 1.0f)) },

	   { PNCT_VERTEX(
		 Vector3(-1.0f, 0.5f, 0.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(0.0f,1.0f)) },
	};

	return true;
}

bool HMinimap::CreateIndexData()
{
	HShapePlane::CreateIndexData();

	return true;
}

bool HMinimap::Create(ID3D11Device* pd3dDevice, T_STR szVS, T_STR szPS, T_STR filename)
{
	m_HDxRT.Set(pd3dDevice);
	g_pd3dDevice = pd3dDevice;
	m_HDxRT.SetRenderTargetView();
	m_HDxRT.SetDepthStencilView();
	m_HDxRT.SetViewport();
	HDxObject::Create(pd3dDevice, szVS, szPS, filename);

	return true;
}

bool HMinimap::Begin(ID3D11DeviceContext * pd3dContext)
{
	//m_HDxRT.Begin(pd3dContext);
	UINT iNumView = 1;

	pd3dContext->RSGetViewports(&iNumView, &m_HDxRT.m_SaveVIewPort);
	pd3dContext->OMGetRenderTargets(1, &m_HDxRT.m_pSaveRTV, &m_HDxRT.m_pSaveDSV);
	/*float clearColor[] = { cosf(g_fGameTimer)*0.5f + 0.5f,
							-cosf(g_fGameTimer)*0.5f + 0.5f,
							sinf(g_fGameTimer)*0.5f + 0.5f,1 };*/

							// 클리어
	ID3D11RenderTargetView* pNull = NULL;
	pd3dContext->OMSetRenderTargets(1, &pNull, NULL);
	m_HDxRT.ClearShaderResources(pd3dContext);

	// 클리어 후 새로 set
	pd3dContext->RSSetViewports(1, &m_HDxRT.m_ViewPort);
	pd3dContext->OMSetRenderTargets(1, &m_HDxRT.m_pRTV, m_HDxRT.m_pDSV);
	float clearColor[] = { 0,0,0,0 };
	//pd3dContext->ClearRenderTargetView(m_HDxRT.m_pRTV, clearColor);
	pd3dContext->ClearDepthStencilView(m_HDxRT.m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	return true;
}

bool HMinimap::End(ID3D11DeviceContext * pd3dContext)
{
	m_HDxRT.End(pd3dContext);
	return true;
}

bool HMinimap::Release()
{
	m_HDxRT.Release();
	return HObject::Release();
}

bool HMinimap::Render(ID3D11DeviceContext* pContext)
{
	HDxObject::Update(pContext);
	PreRender(pContext);
	UINT iStride = sizeof(PNCT_VERTEX);
	UINT iOffset = 0;
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetInputLayout(m_pInputLayout);

	pContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	pContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	pContext->VSSetShader(m_pVertexShader, NULL, 0);
	pContext->PSSetShader(m_pPixelShader, NULL, 0);
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	if (m_pTexture != nullptr)
	{
		pContext->PSSetShaderResources(0, 1, &m_pTexture->m_pTextureSRV);
	}
	if (m_pIndexBuffer == nullptr)
	{
		pContext->Draw(m_VertexList.size(), 0);
	}
	else
	{
		PostRender(pContext);
	}

	return true;
}

bool HMinimap::PostRender(ID3D11DeviceContext * pd3dContext)
{
	pd3dContext->PSSetShaderResources(0, 1, &m_HDxRT.m_pSRV);
	HObject::PostRender(pd3dContext);
	m_HDxRT.ClearShaderResources(pd3dContext);
	return true;
}

HMinimap::HMinimap()
{
}

HMinimap::~HMinimap()
{
}
