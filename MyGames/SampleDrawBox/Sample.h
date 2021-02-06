#pragma once
#include "HCore.h"
#include "HVector.h"
#include <d3dcompiler.h>
#include "WICTextureLoader.h"
#include "HMatrix.h"
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "directxtk.lib")

struct H_VERTEX
{
	HVector3 p;
	HVector2 t;
};

struct P_VERTEX
{
	HVector3 p;
	HVector3 n;
	HVector4 c;
	HVector2 t;

	P_VERTEX() {};
	P_VERTEX(HVector3 p, HVector3 n, HVector4 c, HVector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};

struct HDataCB
{
	HMatrix matWorld;
	HMatrix matView;
	HMatrix matProject;

	float vColor[4];
	float vTime[4];
};

class Sample : public HCore
{

public:
	std::vector<P_VERTEX>		m_VertexList;
	std::vector<DWORD>			m_IndexList;

public:
	HMatrix						m_matWorld;
	HMatrix						m_matView;
	HMatrix						m_matProject;
	HVector3					m_vCameraPos = { -5,-3,0 };
	HVector3					m_vCameraTarget = { 0,0,0 };

public:
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_ConstantBuffer;

	ID3D11InputLayout*			m_pInputLayout;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;

	ID3D11RasterizerState*		m_pRSSSolidBack;
	ID3D11RasterizerState*		m_pRSWireBack;
	ID3D11RasterizerState*		m_pRS;
	D3D11_FILL_MODE				m_FillMode;
	D3D11_CULL_MODE				m_CullMode;

	ID3D11ShaderResourceView*	m_pTextureSRV;
	ID3D11SamplerState*			m_pWrapLinear;

	ID3D11DepthStencilView*		m_pDSV;
	ID3D11DepthStencilState*	m_pDSS;

	HDataCB						m_cbData;

	UINT iNumVertex;
	UINT iNumIndex;

public:
	bool Init() override;
	bool Frame() override;
	bool PreRender() override;
	bool Render() override;
	bool Release() override;

public:
	void CompilerCheck(ID3DBlob* ErrorMsg);
	void SetRasterState();

};

HGAME_RUN;