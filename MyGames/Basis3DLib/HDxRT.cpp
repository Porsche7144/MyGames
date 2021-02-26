#include "HDxRT.h"


HRESULT	HDxRT::SetRenderTargetView()
{

	HRESULT hr;
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	//UINT Width;
	//UINT Height;
	//UINT MipLevels;
	//UINT ArraySize;
	//DXGI_FORMAT Format;
	//DXGI_SAMPLE_DESC SampleDesc;
	//D3D11_USAGE Usage;
	//UINT BindFlags;
	//UINT CPUAccessFlags;
	//UINT MiscFlags;
	textureDesc.Width = 512;
	textureDesc.Height = 512;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}

	hr = g_pd3dDevice->CreateRenderTargetView(pTexture, NULL, &m_pRTV);
	if (FAILED(hr))
	{
		return false;
	}

	hr = g_pd3dDevice->CreateShaderResourceView(pTexture, NULL, &m_pSRV);

	if (pTexture)
	{
		pTexture->Release();
	}

	return hr;
}
HRESULT HDxRT::SetDepthStencilView()
{
	// Depth Texture 생성
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	//UINT Width;
	//UINT Height;
	//UINT MipLevels;
	//UINT ArraySize;
	//DXGI_FORMAT Format;
	//DXGI_SAMPLE_DESC SampleDesc;
	//D3D11_USAGE Usage;
	//UINT BindFlags;
	//UINT CPUAccessFlags;
	//UINT MiscFlags;
	textureDesc.Width = 512;
	textureDesc.Height = 512;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(pTexture, &dsvDesc, &m_pDSV);
	if (FAILED(hr))
	{
		return false;
	}

	if (pTexture)
	{
		pTexture->Release();
	}

	return true;
}



bool HDxRT::Begin(ID3D11DeviceContext * pd3dContext)
{
	UINT iNumView = 1;

	pd3dContext->RSGetViewports(&iNumView, &m_SaveVIewPort);
	pd3dContext->OMGetRenderTargets(1, &m_pSaveRTV, &m_pSaveDSV);
	/*float clearColor[] = { cosf(g_fGameTimer)*0.5f + 0.5f,
							-cosf(g_fGameTimer)*0.5f + 0.5f,
							sinf(g_fGameTimer)*0.5f + 0.5f,1 };*/

	// 클리어
	ID3D11RenderTargetView* pNull = NULL;
	pd3dContext->OMSetRenderTargets(1, &pNull, NULL);
	ClearShaderResources(pd3dContext);

	// 클리어 후 새로 set
	pd3dContext->RSSetViewports(1, &m_ViewPort);
	pd3dContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);
	float clearColor[] = { 0,0,0,1 };
	pd3dContext->ClearRenderTargetView(m_pRTV, clearColor);
	pd3dContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	return true;
}

bool HDxRT::End(ID3D11DeviceContext * pd3dContext)
{
	UINT iNumView = 1;
	pd3dContext->OMSetRenderTargets(1, &m_pSaveRTV, m_pSaveDSV);
	pd3dContext->RSSetViewports(iNumView, &m_SaveVIewPort);

	m_pSaveRTV->Release();
	m_pSaveDSV->Release();

	return true;
}

void HDxRT::Set(ID3D11Device * pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
}

void HDxRT::ClearShaderResources(ID3D11DeviceContext * pd3dContext)
{
	ID3D11ShaderResourceView* pNullViews[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	pd3dContext->VSSetShaderResources(0, 8, pNullViews);
	pd3dContext->GSSetShaderResources(0, 8, pNullViews);
	pd3dContext->PSSetShaderResources(0, 8, pNullViews);
	pd3dContext->PSSetShaderResources(1, 8, pNullViews);
	pd3dContext->PSSetShaderResources(2, 8, pNullViews);
	pd3dContext->PSSetShaderResources(3, 8, pNullViews);
}



bool HDxRT::SetViewport()
{
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = 512;
	m_ViewPort.Height = 512;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	return true;
}

bool HDxRT::Release()
{
	m_pRTV->Release();
	m_pSRV->Release();
	m_pDSV->Release();

	return true;
}