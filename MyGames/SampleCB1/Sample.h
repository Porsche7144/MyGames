#pragma once
#include "HCore.h"
#include "HVertex.h"
#include <d3dcompiler.h>
#include "WICTextureLoader.h"
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "directxtk.lib")

struct P_VERTEX
{
	HVertex3 p;
	HVertex3 n;
	HVertex4 c;
	HVertex2 t;

	P_VERTEX() {};
	P_VERTEX(HVertex3 p, HVertex3 n, HVertex4 c, HVertex2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};

struct HDataCB
{
	float vColor[4];
	float vTime[4];
};

class Sample : public HCore
{

public:
	std::vector<P_VERTEX>		m_VertexList;
	std::vector<DWORD>			m_IndexList;

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

	HDataCB						m_cbData;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

public:
	void CompilerCheck(ID3DBlob* ErrorMsg);
	void SetRasterState();

};

HGAME_RUN;