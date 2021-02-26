#pragma once
#include "HWindow.h"
#include <d3d11.h>
#include "HDxState.h"

class HDevice : public HWindow
{
public:
	Microsoft::WRL::ComPtr<IDXGIFactory>	m_pGIFactory;
	ComPtr<ID3D11Device> 					m_pd3dDevice;
	ComPtr<ID3D11DeviceContext> 			m_pd3dContext;
	ComPtr<IDXGISwapChain> 					m_pSwapChain;
	ComPtr<ID3D11RenderTargetView>			m_pRednerTargetView;
	ComPtr<ID3D11DepthStencilView>			m_pDSV;

	D3D11_VIEWPORT			m_ViewPort;

public:
	HRESULT		CreateGIFactory();
	HRESULT		CreateDevice();
	HRESULT		CreateSwapChain();
	HRESULT		SetRenderTargetView();
	HRESULT     SetDepthStencilView();
	bool		SetViewport();
public:
	bool		Init();
	bool		Frame();
	bool		PreRender();
	bool		Render();
	bool		PostRender();
	bool		Release();
public:
	void		SetMode(bool bFullScreen);
	virtual void ResizeDevice(UINT w, UINT h) override;
	virtual HRESULT DeleteDXResource();
	virtual HRESULT CreateDXResource(UINT w, UINT h);
public:
	HDevice();
	virtual ~HDevice();
};

