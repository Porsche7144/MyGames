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

struct H_SPHERE
{
	Vector3 vCenter;
	float fRadius;
};

struct H_BOX
{
	// Common
	Vector3 vCenter;
	Vector3 vPos[8];

	// AABB
	Vector3 vMax;
	Vector3 vMin;

	// OBB
	Vector3 vAxis[3];
	// 크기/넓이
	float fExtent[3];
};


namespace HBASIS_CORE_LIB
{
	////////////////////////// 아래의 경고가 있을 경우 사용한다.
	// 이와 같은 경고는 이미 쉐이더 파이프라인에 할당(리소스 및 상태값들이)되어 사용 중일 경우에 발생한다.
	//D3D11 WARNING : ID3D11DeviceContext::SOSetTargets : Resource being set to SO buffer slot 0 is still bound on input![STATE_SETTING WARNING #10: DEVICE_SOSETTARGETS_HAZARD]
	//D3D11 WARNING : ID3D11DeviceContext::SOSetTargets : Forcing Vertex Buffer slot 0 to NULL.[STATE_SETTING WARNING #1: DEVICE_IASETVERTEXBUFFERS_HAZARD]
	void		ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);

	HRESULT	CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT D3DX11CompileFromFile(LPCWSTR pSrcFile, CONST D3D_SHADER_MACRO* pDefines, LPD3DINCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2,
		/*ID3DX11ThreadPump* pPump, */ID3DBlob** ppShader, ID3DBlob** ppErrorMsgs, HRESULT* pHResult);
	ID3D11VertexShader* LoadVertexShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11PixelShader*  LoadPixelShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, const char *pFuntionName = 0, bool bBinary = false, ID3DBlob** pRetBlob = nullptr);
	ID3D11GeometryShader*  LoadGeometryShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11HullShader* LoadHullShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11DomainShader*  LoadDomainShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11ComputeShader*  LoadComputeShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);


	ID3D11InputLayout* CreateInputlayout(ID3D11Device*  pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device*  pd3dDevice, void *vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic = false);
	ID3D11Buffer* CreateIndexBuffer(ID3D11Device*  pd3dDevice, void *indices, UINT iNumIndex, UINT iSize, bool bDynamic = false);
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device*  pd3dDevice, void *data, UINT iNumIndex, UINT iSize, bool bDynamic = false);

	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, const TCHAR* strFilePath);
	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, ID3D11DeviceContext*    pContext, const TCHAR* strFilePath);
	ID3D11DepthStencilView* CreateDepthStencilView(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);


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
		ID3D11Device*				g_pd3dDevice;

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

}
