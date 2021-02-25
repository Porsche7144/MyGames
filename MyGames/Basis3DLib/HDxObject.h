#pragma once
#include "HStd.h"
#include "HDxState.h"
#include "SimpleMath.h"
#include "HTexture.h"

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

class HDxObject
{

public:
	std::vector<PNCT_VERTEX>		m_VertexList;
	std::vector<DWORD>			m_IndexList;

	Matrix						m_matWorld;
	Matrix						m_matView;
	Matrix						m_matProject;
	Matrix						m_matScale;
	Matrix						m_matRotation;

	HTexture* m_pTexture;

public:
	ID3DBlob*					pVSObj;
	ID3D11Device*				m_pd3dDevice;

	HDataCB						m_cbData;

	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_ConstantBuffer;

	ID3D11InputLayout*			m_pInputLayout;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;

	// 유니코드 불가
	C_STR m_szVertexShader;
	C_STR m_szPixelShader;

	UINT iNumVertex;
	UINT iNumIndex;
	UINT m_iTopology;

	Vector3		m_vPos = { 0,0,0 };
	Vector3		m_vTarget = { 0,0,0 };
	Vector3		m_vLook;
	Vector3		m_vUp;
	Vector3		m_vRight;
	

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
	virtual bool PreRender(ID3D11DeviceContext* pContext);
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	virtual bool Release();
public:
	HDxObject();
	virtual ~HDxObject();
};

