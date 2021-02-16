#pragma once
#include "HStd.h"
#include "HDxState.h"
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

struct P_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;

	P_VERTEX() {};
	P_VERTEX(Vector3 p, Vector3 n, Vector4 c, Vector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};

struct PNCT_VERTEX
{
	Vector3		p;
	Vector3		n;
	Vector4		c;
	Vector2     t;
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(Vector3		vp,
		Vector3		vn,
		Vector4		vc,
		Vector2     vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};

struct HDataCB
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProject;

	float vColor[4];
	float vTime[4];
};

class HShape
{

public:
	std::vector<PNCT_VERTEX>		m_VertexList;
	std::vector<DWORD>			m_IndexList;

	Matrix						m_matWorld;
	Matrix						m_matView;
	Matrix						m_matProject;

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

	// �����ڵ� �Ұ�
	C_STR m_szVertexShader;
	C_STR m_szPixelShader;

	UINT iNumVertex;
	UINT iNumIndex;
	UINT m_iTopology;

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
	virtual bool SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj);

	void CompilerCheck(ID3DBlob* ErrorMsg);

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool Release();
public:
	HShape();
	virtual ~HShape();
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
	bool Draw(ID3D11DeviceContext* pContext, Vector3 p, Vector3 e, Vector4 c = Vector4(1, 0, 0, 1));

public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	HShapeLine();
	virtual ~HShapeLine();
};
