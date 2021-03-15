#pragma once
#include <fbxsdk.h>
#include "HObject.h"
#include <unordered_map>

using namespace DirectX::SimpleMath;

class HFbxObj
{
public:
	static FbxManager*	g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFbxScene;
	std::unordered_map<std::string, Matrix> m_dxMatrixMap;

public:
	bool Load(std::string FileName);
	bool LoadFBX(std::string FileName);
	bool Initialize(std::string FileName);
	void PreProcess(FbxNode* pNode);
	void ParseNode(FbxNode* pNode, FbxScene* pScene);
	void ParseAnimation(FbxScene * pScene);

public:
	HFbxObj();
};

