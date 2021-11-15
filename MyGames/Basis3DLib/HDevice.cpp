#include "HDevice.h"

namespace HBASIS_CORE_LIB
{
	ID3D11Device* g_pd3dDevice = nullptr;
	// 즉시 Context
	ID3D11DeviceContext* g_pImmediateContext = nullptr;
}

void	HDevice::SetMode(bool bFullScreen)
{
	m_bFullScreen = bFullScreen;
	m_pSwapChain->SetFullscreenState(m_bFullScreen, NULL);
	if (m_bFullScreen == FALSE)
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}
}
void HDevice::ResizeDevice(UINT w, UINT h)
{
	if (m_pd3dDevice.Get() == NULL)  return;

	HRESULT hr = S_OK;

	DeleteDXResource();

	m_pd3dContext->OMSetRenderTargets(0, NULL, NULL);
	if (m_pRednerTargetView.Get())
	{
		m_pRednerTargetView->Release();
	}
	if (m_pDSV.Get())
	{
		m_pDSV->Release();
	}

	
	m_pSwapChain->GetDesc(&m_pSwapChainDesc);
	hr = m_pSwapChain->ResizeBuffers(
		m_pSwapChainDesc.BufferCount,
		w,
		h,
		m_pSwapChainDesc.BufferDesc.Format,
		m_pSwapChainDesc.Flags);
	if (FAILED(hr))
	{
		return;
	}

	m_pSwapChain->GetDesc(&m_pSwapChainDesc);
	m_rtClient.right = m_pSwapChainDesc.BufferDesc.Width;
	m_rtClient.bottom = m_pSwapChainDesc.BufferDesc.Height;
	g_rtClient = m_rtClient;

	SetRenderTargetView();
	SetDepthStencilView();
	SetViewport();

	CreateDXResource(w, h);
}
HRESULT HDevice::DeleteDXResource()
{
	return S_OK;
}
HRESULT HDevice::CreateDXResource(UINT w, UINT h)
{
	return S_OK;
}
HRESULT HDevice::CreateGIFactory()
{
	if (m_pd3dDevice.Get() == NULL) return E_FAIL;
	HRESULT hr;
	ComPtr<IDXGIDevice> pDXGIDevice;
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)pDXGIDevice.GetAddressOf());

	ComPtr<IDXGIAdapter> pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)pDXGIAdapter.GetAddressOf());

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)m_pGIFactory.GetAddressOf());

	return S_OK;
}
HRESULT		HDevice::CreateDevice()
{
	//D2D RT
	UINT	Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL	pFeatureLevels[] =
	{
		//D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};
	// 그래픽카드
	IDXGIAdapter*		 pAdapter = nullptr;
	D3D_DRIVER_TYPE		DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	HMODULE				Software = NULL; // 외부모듈 사용유무

	//UINT				FeatureLevels = ARRAYSIZE(pFeatureLevels);
	UINT				FeatureLevels = sizeof(pFeatureLevels) / sizeof(pFeatureLevels[0]);
	UINT				SDKVersion = D3D11_SDK_VERSION;
	D3D_FEATURE_LEVEL   OutputFeatureLevel;

	HRESULT hr = S_OK;
	for (int iType = 0; iType < ARRAYSIZE(DriverType); iType++)
	{
		hr = D3D11CreateDevice(
			pAdapter,
			DriverType[iType],
			Software,
			Flags,
			pFeatureLevels,
			FeatureLevels,
			SDKVersion,
			m_pd3dDevice.GetAddressOf(),
			&OutputFeatureLevel,
			m_pd3dContext.GetAddressOf());

		if (SUCCEEDED(hr))
		{
			break;
		}
	}

	g_pd3dDevice = m_pd3dDevice.Get();
	g_pImmediateContext = m_pd3dContext.Get();

	return hr;
}
HRESULT		HDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	ZeroMemory(&pSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	pSwapChainDesc.BufferDesc.Width = g_rtClient.right;
	pSwapChainDesc.BufferDesc.Height = g_rtClient.bottom;
	pSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	pSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	pSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	pSwapChainDesc.SampleDesc.Count = 1;
	pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	pSwapChainDesc.BufferCount = 1;
	pSwapChainDesc.OutputWindow = g_hWnd;
	pSwapChainDesc.Windowed = true;
	//pSwapChainDesc.SwapEffect;
	pSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = m_pGIFactory->CreateSwapChain(
		m_pd3dDevice.Get(),
		&pSwapChainDesc,
		m_pSwapChain.GetAddressOf());
	return hr;
}
HRESULT		HDevice::SetRenderTargetView()
{
	ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)pBackBuffer.GetAddressOf());
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
		m_pRednerTargetView.GetAddressOf());

	return hr;
}
HRESULT HDevice::SetDepthStencilView()
{
	// Depth Texture 생성

	ComPtr<ID3D11Texture2D> pTexture = nullptr;
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
	textureDesc.Width = g_rtClient.right;
	textureDesc.Height = g_rtClient.bottom;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = m_pd3dDevice->CreateTexture2D(&textureDesc, NULL, pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(pTexture.Get(), &dsvDesc, m_pDSV.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	//if (pTexture.Get())
	//{
	//	pTexture->Release();
	//}

	return true;
}

bool HDevice::SetViewport()
{
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = g_rtClient.right;
	m_ViewPort.Height = g_rtClient.bottom;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	return true;
}
bool HDevice::Init()
{
	if (FAILED(CreateDevice()))
	{
		return false;
	}
	if (FAILED(CreateGIFactory()))
	{
		return false;
	}
	if (FAILED(CreateSwapChain()))
	{
		return false;
	}
	if (FAILED(SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(SetDepthStencilView()))
	{
		return false;
	}

	if (SetViewport() == false)
	{
		return false;
	}

	HDxState::SetState(m_pd3dDevice.Get());

	if (FAILED(m_pGIFactory->MakeWindowAssociation(m_hWnd,
		DXGI_MWA_NO_WINDOW_CHANGES |
		DXGI_MWA_NO_ALT_ENTER)))
	{
		return false;
	}
	return true;
}
bool HDevice::Frame()
{
	return true;
}
bool HDevice::PreRender()
{
	if (g_pImmediateContext)
	{
		g_pImmediateContext->RSSetViewports(1, &m_ViewPort);
		g_pImmediateContext->OMSetRenderTargets(1, m_pRednerTargetView.GetAddressOf(), m_pDSV.Get());
		/*float clearColor[] = { cosf(g_fGameTimer)*0.5f + 0.5f,
								-cosf(g_fGameTimer)*0.5f + 0.5f,
								sinf(g_fGameTimer)*0.5f + 0.5f,1 };*/
		float clearColor[] = { 0.25f, 0.25f, 0.25f ,1.0f };
		g_pImmediateContext->ClearRenderTargetView(m_pRednerTargetView.Get(), clearColor);
		g_pImmediateContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_pImmediateContext->RSSetState(HDxState::g_pRSEdit.Get());
		g_pImmediateContext->PSSetSamplers(0, 1, &HDxState::g_pSSWrapLinear);
		g_pImmediateContext->OMSetDepthStencilState(HDxState::g_pDSSDepthEnable, 0);
	}
	return true;
}
bool HDevice::Render()
{
	return true;
}
bool HDevice::PostRender()
{
	if (m_pSwapChain.Get())
	{
		m_pSwapChain->Present(0, 0);
	}
	return true;
}
bool HDevice::Release()
{
	HDxState::Release();
	return true;
}
HDevice::HDevice()
{
	m_pGIFactory = nullptr;
	m_pd3dDevice = nullptr;
	m_pd3dContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRednerTargetView = nullptr;
}
HDevice::~HDevice()
{

}