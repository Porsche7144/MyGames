#pragma once
#include "HCore.h"
#include "HShape.h"
#include "HObject.h"
#include "HheightMap.h"
#include "HMinimap.h"
#include "HQuadTree.h"
#include "HPicking.h"
#include "HModelViewCamera.h"
//#include "HModel.h"
#include "HTextureMap.h"

#pragma comment(lib, "directxtk.lib")

struct SaveData
{
	TCHAR* fileName;
	int iTileCount;
	float fCellCount;
	float fScale;
	std::vector<Vector3> m_SaveVertexList;
	int iListSize;

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
	H_BOX			m_TBoxBase;
	HPicking		m_Picking;
	//HModel			m_pObj;
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

	ID3D11ShaderResourceView* m_pTextureSRV[5];

	Vector3 beforePos;

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

	std::vector<HObject*> m_ObjList;
	std::vector<HNode*> m_SelectNode;
	std::vector<HNode*> m_ControlNode;
	std::vector<Matrix> m_ModelMatrixList;

public:

	Matrix						m_matBoxWorld;
	Matrix						m_matPlaneWorld;
	Matrix						m_matLineWorld;

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