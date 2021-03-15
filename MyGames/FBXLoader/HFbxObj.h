#pragma once
#include <fbxsdk.h>
#include "HObject.h"
#include <unordered_map>

using namespace DirectX::SimpleMath;

typedef std::unordered_map<FbxNode*, HObject*> hMeshMap;
class HFbxObj
{
public:
	static FbxManager*	g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFbxScene;
	std::unordered_map<std::string, Matrix> m_dxMatrixMap;
	hMeshMap m_hMeshMap;

public:
	bool Load(std::string FileName);
	bool LoadFBX(std::string FileName);
	bool Initialize(std::string FileName);
	void PreProcess(FbxNode* pNode);
	void ParseNode(FbxNode* pNode, Matrix matParent);
	void ParseAnimation(FbxScene * pScene);
	void ParseMash(FbxNode* pNode, FbxMesh* pMesh, HObject* pObj);
	Matrix ParseTransform(FbxNode* pNode, Matrix& matParentWorld);

public:
	HFbxObj();
};

