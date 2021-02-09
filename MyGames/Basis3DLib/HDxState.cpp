#include "HDxState.h"

ID3D11RasterizerState*		HDxState::m_pRSSSolidBack = nullptr;
ID3D11RasterizerState*		HDxState::m_pRSWireBack = nullptr;
ID3D11RasterizerState*		HDxState::m_pRS = nullptr;
ID3D11SamplerState*			HDxState::m_pWrapLinear = nullptr;
ID3D11DepthStencilState*	HDxState::m_pDSS = nullptr;

D3D11_FILL_MODE				HDxState::m_FillMode = D3D11_FILL_SOLID;
D3D11_CULL_MODE				HDxState::m_CullMode = D3D11_CULL_NONE;

bool HDxState::Set(ID3D11Device* pDevice)
{
	// Depth Stencil State

	D3D11_DEPTH_STENCIL_DESC dsdDesc;
	ZeroMemory(&dsdDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsdDesc.DepthEnable = TRUE;
	dsdDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsdDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	HRESULT hr = pDevice->CreateDepthStencilState(&dsdDesc, &m_pDSS);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1;
	samplerDesc.MinLOD = FLT_MIN;
	samplerDesc.MaxLOD = FLT_MAX;
	hr = pDevice->CreateSamplerState(&samplerDesc, &m_pWrapLinear);
	if (FAILED(hr))
	{
		return false;
	}

	// RASTERIZER 상태

	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	hr = pDevice->CreateRasterizerState(&rd, &m_pRSSSolidBack);
	if (FAILED(hr))
	{
		return false;
	}

	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	hr = pDevice->CreateRasterizerState(&rd, &m_pRSWireBack);
	if (FAILED(hr))
	{
		return false;
	}

	// update

	m_FillMode = D3D11_FILL_SOLID;
	m_CullMode = D3D11_CULL_NONE;
	SetRasterState(pDevice);

	return true;
}

bool HDxState::SetRasterState(ID3D11Device* pDevice)
{
	HRESULT hr;
	// RASTERIZER 상태
	if (m_pRS != nullptr)
	{
		m_pRS->Release();
	}
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = m_FillMode;
	rd.CullMode = m_CullMode;
	rd.DepthClipEnable = TRUE;

	hr = pDevice->CreateRasterizerState(&rd, &m_pRS);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool HDxState::Release()
{
	m_pDSS->Release();
	m_pWrapLinear->Release();
	m_pRS->Release();
	if (m_pRSSSolidBack != nullptr)
	{
		m_pRSSSolidBack->Release();
	}
	if (m_pRSWireBack != nullptr)
	{
		m_pRSWireBack->Release();
	}
	return true;
}
