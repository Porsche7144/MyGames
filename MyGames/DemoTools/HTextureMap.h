#pragma once
#include "HShape.h"
#include "HDxRT.h"
#include "HMap.h"
#include "HQuadTree.h"

enum SplattingTexture
{
	SPLATTING_NUM_1 = 1,
	SPLATTING_NUM_2 = 2,
	SPLATTING_NUM_3 = 3,
	SPLATTING_NUM_4 = 4,
};

class HTextureMap : public HShapePlane
{
public:
	HShapePlane m_Plane;
	HDxRT m_HDxRT;
	D3D11_VIEWPORT		m_RightViewPort;
	SplattingTexture	m_SplattingNum;

	ID3D11RenderTargetView* m_pRTV;
	ID3D11ShaderResourceView* m_pSRV;
	ID3D11DepthStencilView* m_pDSV;

	ID3D11RenderTargetView* m_pSaveRTV;
	ID3D11DepthStencilView* m_pSaveDSV;
	D3D11_VIEWPORT		m_SaveVIewPort;
	
	Vector3 PickPos;
	ID3D11Texture2D* pTexture;
	ID3D11Texture2D* pStaging;

	float m_fRadius;
	Vector2 beforePos;

public:
	void PickRenderTextureData(HMap* map, ID3D11Texture2D* Texture2D, ID3D11DeviceContext* pContext, 
								Vector3 pick, int SplattingNum);

public:
	bool Create(HMap* map, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, T_STR szVS, T_STR szPS, T_STR filename);
	bool CreateVertexData() override;
	bool CreateIndexData() override;

public:
	virtual bool	Begin(ID3D11DeviceContext* pd3dContext);
	virtual bool	End(ID3D11DeviceContext* pd3dContext);
	virtual bool	Release() override;
	virtual bool	Render(ID3D11DeviceContext* pd3dContext) override;
	virtual bool	Frame(HMap* map, ID3D11DeviceContext* pContext);
	virtual bool	PostRender(ID3D11DeviceContext* pd3dContext) override;
	HRESULT	SetRenderTargetView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT	SetStaging(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT SetDepthStencilView();
	void ClearShaderResources(ID3D11DeviceContext * pd3dContext);
	bool SetViewport();
	HRESULT CreateStagingTexture2D(HMap* map, ID3D11DeviceContext* pContext);
	void WriteTextureData(HMap* map, ID3D11DeviceContext* pContext);
	void WriteTextureDataAlphaZero(HMap* map, ID3D11DeviceContext* pContext);
	void SetRadius(float radius);

public:
	HTextureMap();
	virtual ~HTextureMap();

};

