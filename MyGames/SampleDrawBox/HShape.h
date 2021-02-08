#pragma once
#include "HStd.h"
#include "HMAtrix.h"
#include "WICTextureLoader.h"

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

class HShape
{

public:
	std::vector<P_VERTEX>		m_VertexList;
	std::vector<DWORD>			m_IndexList;

	HMatrix						m_matWorld;
	HMatrix						m_matView;
	HMatrix						m_matProject;

public:
	ID3DBlob*					pVSObj;
	ID3D11Device*				m_pd3dDevice;

	HDataCB						m_cbData;
	ID3D11ShaderResourceView*	m_pTextureSRV;

	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_ConstantBuffer;

	ID3D11InputLayout*			m_pInputLayout;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;

	UINT iNumVertex;
	UINT iNumIndex;

public:
	virtual bool Create(ID3D11Device* pDevice, T_STR vs, T_STR ps, T_STR texture);
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	virtual bool CreateConstantBuffer();
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual bool CreateInputLayOut();
	virtual bool Update(ID3D11DeviceContext* pd3dContext);
	virtual bool LoadShader(T_STR vs, T_STR ps);
	virtual bool LoadTexture(T_STR texture);
	virtual bool SetMatrix(HMatrix* pWorld, HMatrix* pView, HMatrix* pProj);

	void CompilerCheck(ID3DBlob* ErrorMsg);

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool Release();

};

class HShapeBox : public HShape
{
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	HShapeBox();
	virtual ~HShapeBox();
};

class HShapePlane : public HShape
{
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	HShapePlane();
	virtual ~HShapePlane();
};

class HShapeLine : public HShape
{
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	HShapeLine();
	virtual ~HShapeLine();
};

