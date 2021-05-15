//#pragma once
//#include "HModelObj.h"
//#include "HTimer.h"
//
//using namespace DirectX::SimpleMath;
//
//static Matrix DxConvertMatrix(Matrix m)
//{
//	Matrix mat;
//
//	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
//	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
//	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
//	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
//
//	mat._14 = mat._24 = mat._34 = 0.0f;
//	mat._44 = 1.0f;
//
//	return mat;
//};
//
//static Matrix ConvertMatrixA(const FbxMatrix& matrix)
//{
//	Matrix matResult;
//	auto fData = reinterpret_cast<float*>(&matResult);
//	auto pSrcData = reinterpret_cast<const DOUBLE*>(&matrix);
//	for (DWORD i = 0; i < 16; ++i)
//	{
//		fData[i] = (float)pSrcData[i];
//	}
//	return matResult;
//};
//
//typedef std::unordered_map<FbxNode*, HModelObj*> hNodeMap;
//typedef std::vector<HModelObj*>	hNodeList;
//
//class HFbxObj : public HObject
//{
//public:
//	HScene    m_AnimScene;
//	float	  m_fTick = 0.0f;
//	static FbxManager*	g_pSDKManager;
//	FbxImporter*		m_pFbxImporter;
//	FbxScene*			m_pFbxScene;
//	FbxNode*			m_pFbxRootNode;
//	std::unordered_map<std::string, Matrix> m_dxMatrixMap;
//	std::unordered_map<FbxNode*, int> m_pFbxNodeMap;
//	std::vector<FbxNode*> m_pFbxNodeList;
//	std::vector<Matrix> m_MatrixList;
//	hNodeMap m_hNodeMap;
//	hNodeList m_hNodeList;
//
//	ComPtr<ID3D11Buffer> m_pBoneBuffer;
//
//public:
//	bool Load(std::string FileName);
//	bool LoadFBX(std::string FileName);
//	bool Initialize(std::string FileName);
//	void PreProcess(FbxNode* pNode);
//	void ParseNode(FbxNode* pNode, Matrix matParent, HModelObj* pParentObj);
//	void ParseAnimation(FbxScene * pScene);
//	void ParseMesh(FbxNode* pNode, FbxMesh* pMesh, HModelObj* pObj);
//	Matrix ParseTransform(FbxNode* pNode, Matrix& matParentWorld);
//	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,	int vertexIndex, int uvIndex, FbxVector2& uv);
//	FbxColor ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet,
//		DWORD dwDCCIndex, DWORD dwVertexIndex);
//	FbxVector4 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int VertexCounter);
//	std::string ParseMaterial(FbxSurfaceMaterial* pMtrl);
//	bool ParseMeshSkinningMap(const FbxMesh * pMesh, std::vector<HWeight>& skindata, HModelObj* pObj);
//	void ParseNodeAnimation(FbxNode* pNode);
//	void ParseAnimStack(FbxScene* pScene, FbxString* pStrAnimStackName);
//	bool CreateInputLayOut() override;
//	void AddKey(FbxNode* pNode, FbxMatrix matGlobal, float CurrentTime);
//
//public:
//	HFbxObj();
//};
//
