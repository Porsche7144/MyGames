#pragma once
#include "HdxObject.h"

class HDxRT
{
public:
	ID3D11Device* m_pd3dDevice;
	ID3D11RenderTargetView* m_pRTV;
	ID3D11ShaderResourceView* m_pSRV;
	ID3D11DepthStencilView* m_pDSV;
	D3D11_VIEWPORT		m_ViewPort;

	ID3D11RenderTargetView* m_pSaveRTV;
	ID3D11DepthStencilView* m_pSaveDSV;
	D3D11_VIEWPORT		m_SaveVIewPort;


public:
	virtual HRESULT	SetRenderTargetView();
	virtual HRESULT SetDepthStencilView();
	virtual bool	Begin(ID3D11DeviceContext* pd3dContext);
	virtual bool	End(ID3D11DeviceContext* pd3dContext);
	virtual void	Set(ID3D11Device* pd3dDevice);
	virtual void	ClearShaderResources(ID3D11DeviceContext* pd3dContext);
	virtual bool	SetViewport();

public:
	bool	Release();
};

