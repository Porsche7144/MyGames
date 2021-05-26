#pragma once
#include "HCore.h"
#include "HShape.h"
#include "HObject.h"
#include "HheightMap.h"
#include "HMinimap.h"
#include "HQuadTree.h"
#include "HPicking.h"
#include "HModelViewCamera.h"
#include "HModel.h"
#include "HTextureMap.h"
#include "HSelect.h"

#pragma comment(lib, "directxtk.lib")
#define TEXTURE_COUNT 4

struct SaveData
{
	TCHAR* fileName;
	int iTileCount;
	float fCellCount;
	float fScale;
	std::vector<Vector3> m_SaveVertexList;
	int iListSize;
};

struct ColisionSphere
{
	Matrix mat;
	HModel pModel;
	H_SPHERE sphere;
};

struct BufferType
{
	Vector3 vPickPos;
	float fRadius;
	int iIndex; // RGBA
	float fTexWidth;
	float fTexHeight;
};

class HBoxUser : public HShapeBox
{
public:
	void FrontMovement(float fDir);
	void RightMovement(float fDir);
	void UpMovement(float fDir);
	void RightBase(float fDir);
	void FrontBase(float fDir);
	void UpBase(float fDir);
};

class Sample : public HCore
{
public:
	SaveData		m_Save;
	HQuadTree		m_QuadTree;
	HShapeBox		m_ShapeBox;
	HPicking		m_Picking;
	HModel			m_pObj;
	HSelect			m_Select;

	int isave;
	HMap				m_Map;
	HMinimap			m_Minimap;
	HTextureMap			m_TextureMap;
	HTextureMap			m_AlphaZeroTexture;

	HBoxUser			m_BoxShape;
	HBoxUser			m_UserShape;
	HShapePlane			m_PlaneShape;
	Vector4				m_vDirValue;
	HModelViewCamera	m_ModelCamera;

	HCamera m_TopCamera;

	Vector3 vPickRayDir;
	Vector3 vPickRayOrigin;
	Vector3 pick;

	Vector3 v0, v1, v2, vNormal, vEnd;
	Vector3 list[3];
	
	ColisionSphere collision;
	ID3D11ShaderResourceView* m_pTextureSRV[5];

	// Object
public:
	std::vector<ColisionSphere> m_ColisionList;
	bool m_bObjInitState = false;
	bool m_bObjFrameState = false;
	bool m_bCreateObj = false;
	bool m_bObjDelete = false;
	bool m_bMoveObj = false;
	bool m_bScale = false;
	bool m_bRotation = false;
	int SelectNum = 0;
	int PickingSelect = 0;
	float m_ScaleCount;
	float m_RotationCount;


	// Map
public:
	bool m_bIncreaseGround = false;
	bool m_bDecreaseGround = false;
	bool m_bOriginGround = false;
	bool m_bFlatGrond = false;
	bool m_bSplattingState = false;
	bool m_bFieldUpdateState = false;
	bool m_SaveData = false;
	bool m_LoadData = false;
	bool m_SaveTexture = false;

	float t, u, v = 0;
	bool m_bSelect;
	static int m_iTileCount;
	static float m_fCellCount;
	static float m_fScale;
	float m_fRadius;
	float m_fSpeed;
	int	  m_iSplattingNum;
	TCHAR* Filename;
	ID3D11Texture2D* m_LoadTexture = nullptr;

	std::string m_FileName;

	Vector3 beforePos;
	std::vector<HObject*> m_ObjList;
	std::vector<HNode*> m_SelectNode;
	std::vector<HNode*> m_ControlNode;
	std::vector<Matrix> m_ModelMatrixList;

public:

	Matrix						m_matBoxWorld;
	Matrix						m_matPlaneWorld;
	Matrix						m_matLineWorld;

public:
	// Compute Shader
	HTexture* m_pSplatting[TEXTURE_COUNT]; // 스플래팅 이미지 4장
	int m_iTextureSizeX = 1024;
	int m_iTextureSizeY = 1024;
	BufferType m_SelectData[TEXTURE_COUNT]; 
	ComPtr<ID3D11ComputeShader> m_pCS;

	HRESULT InitWork();
	HRESULT CreateComputeShader(ID3D11Device* pDevice, LPCWSTR srcFile, LPCSTR FuncName, ID3D11ComputeShader** ppShaderOut);
	void RunComputeShader(ID3D11DeviceContext* pContext, ID3D11ComputeShader* pCS, ID3D11ShaderResourceView** ppSRV,
						  ID3D11UnorderedAccessView** ppUAV, int iNumView, UINT X, UINT Y, UINT Z);

	// 구조화 버퍼
	ComPtr<ID3D11Buffer> m_pSelectBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pSelectBufferSRV;

	HRESULT CreateStructuredBuffer(ID3D11Device* pDevice, UINT uElementSize, UINT uCount,
								   VOID* pInitData, ID3D11Buffer** ppBufOut);
	HRESULT CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut);

	// 알파텍스쳐
	ComPtr<ID3D11Texture2D> m_pAlphaTexture;
	ComPtr<ID3D11ShaderResourceView> m_pAlphaTextureSRV;
	ComPtr<ID3D11UnorderedAccessView> m_pAlphaTextureUAV;

	ComPtr<ID3D11Texture2D> m_pAlphaTextureCopy;
	ComPtr<ID3D11ShaderResourceView> m_pAlphaTextureSRVCopy;

	HRESULT CreateTextureUAV(ID3D11Device* pDevice, int iWidth, int iHeight);


public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool PostRender() override;
	bool Release() override;
	TCHAR* Sample::StringToTCHAR(string& s);


public:
	bool GetIntersection(HNode* pNode);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SaveMapData();
	void LoadMapData(string filename);
	void SaveTextureFile(ID3D11Texture2D* texture, T_STR name);
	ID3D11Texture2D* LoadTexturetMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* pFilename);

public:
	Sample()
	{
		t, u, v = 0;
	};
	~Sample() {};

};










//#pragma once
//#include "HCore.h"
//#include "HMap.h"
//class Sample : public HCore
//{
//public:
//	HMap m_CustomMap;
//	HCamera* m_MainCamera;
//	HShapeLine m_LineDraw;
//
//	Matrix						m_matBoxWorld;
//	Matrix						m_matPlaneWorld;
//	Matrix						m_matLineWorld;
//	static int m_iTileCount;
//
//public:
//	bool Init();
//	bool Frame() override;
//	bool Render() override;
//	bool Release() override;
//
//public:
//	Sample();
//	virtual ~Sample();
//};