#pragma once
#include "HCore.h"
#include "HShape.h"
#include "HObject.h"
#include "HModelViewCamera.h"
#include "HheightMap.h"
#include "HMinimap.h"
#include "HQuadTree.h"
#include "HSky.h"

#pragma comment(lib, "directxtk.lib")

const int g_iNumLight = 3;

struct LIGHT_CONSTANT_BUFFER
{
	// 메테리얼
	Vector4 g_cAmbientMaterial[g_iNumLight];
	Vector4 g_cDiffuseMaterial[g_iNumLight];
	Vector4 g_cSpecularMaterial[g_iNumLight];
	Vector4 g_cEmissionMaterial[g_iNumLight];

	// 라이트컬러
	Vector4 g_cAmbientLightColor[g_iNumLight];
	Vector4 g_cDiffuseLightColor[g_iNumLight];
	Vector4 g_cSpecularLightColor[g_iNumLight];
};

struct LIGHT_CONSTANT_BUFFER2
{
	Matrix	g_matInvWorld[g_iNumLight];
	Vector4 g_vLightDir[g_iNumLight];
	Vector4 g_vLightPos[g_iNumLight];
	Vector4 g_vEyeDir[g_iNumLight];
	Vector4 g_vEyePos[g_iNumLight];
};

struct LIGHT_CONSTANT_BUFFER3
{
	Vector4 g_vSpotInfo[g_iNumLight];
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
	Vector3 m_vLightVector;
	LIGHT_CONSTANT_BUFFER	m_cbLight1;
	LIGHT_CONSTANT_BUFFER2	m_cbLight2;
	LIGHT_CONSTANT_BUFFER3	m_cbLight3;
	ComPtr<ID3D11Buffer> m_pConstantLightBuffer[g_iNumLight];

public:
	HQuadTree		m_QuadTree;
	H_BoxObject*	m_pObject;
	H_BOX			m_TBoxBase;
	HSky			m_Sky;

	HheightMap m_Map;
	HMinimap m_Minimap;
	HBoxUser			m_BoxShape;
	HBoxUser			m_UserShape;
	HShapePlane			m_PlaneShape;
	Vector4				m_vDirValue;
	HModelViewCamera	m_ModelCamera;

	ComPtr<ID3D11ShaderResourceView> m_pTexture[4];

	HCamera m_TopCamera;

	std::vector<HObject*> m_ObjList;

public:

	Matrix						m_matBoxWorld;
	Matrix						m_matPlaneWorld;
	Matrix						m_matLineWorld;

public:
	Matrix* TD3DXMatrixShadow(Matrix *pout,
		Vector4 *plight,
		Vector4 *pplane);
	Matrix CreateMatrixShadow(
		Vector4* pPlane,
		Vector4* pLight);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//bool DrawQuadLine(HNode* pNode);
	void DrawObject(Matrix* pView, Matrix* pProj);

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool PostRender() override;
	bool Release() override;

public:
	void SetRasterState();

};

HGAME_RUN;