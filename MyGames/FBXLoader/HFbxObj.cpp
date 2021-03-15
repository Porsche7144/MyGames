#include "HFbxObj.h"
FbxManager*	HFbxObj::g_pSDKManager = nullptr;

bool HFbxObj::Load(std::string FileName)
{
	if (LoadFBX(FileName))
	{
		return true;
	}

	return false;
}

bool HFbxObj::LoadFBX(std::string FileName)
{
	if (Initialize(FileName) == false)
	{
		return false;
	}

	FbxNode* pFbxRootNode = m_pFbxScene->GetRootNode();
	PreProcess(pFbxRootNode);
	ParseNode(pFbxRootNode, m_pFbxScene);
	ParseAnimation(m_pFbxScene);

	return true;
}

bool HFbxObj::Initialize(std::string FileName)
{
	// Manager 생성, IO세팅
	if (g_pSDKManager == nullptr)
	{
		g_pSDKManager = FbxManager::Create();
		if (g_pSDKManager == nullptr) return false;

		FbxIOSettings* ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
		if (ios == nullptr) return false;

		g_pSDKManager->SetIOSettings(ios);
	}

	// Importer 생성
	if (m_pFbxImporter == nullptr)
	{
		m_pFbxImporter = FbxImporter::Create(g_pSDKManager, "");
		if (m_pFbxImporter == nullptr) return false;
	}

	// Scene 생성
	if (m_pFbxScene == nullptr)
	{
		m_pFbxScene = FbxScene::Create(g_pSDKManager, "");
		if (m_pFbxScene == nullptr) return false;
	}

	// 사용할 파일 이름과 IOSettings를 제공하여 임포터를 초기화
	bool bRet = m_pFbxImporter->Initialize(FileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;
	
	// Scene을 가져옴.
	bRet = m_pFbxImporter->Import(m_pFbxScene);

	return true;
}

void HFbxObj::PreProcess(FbxNode * pNode)
{
	// 카메라와 라이트 정보는 불필요하므로 걸러냄.
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}

	Matrix mat = Matrix::Identity;
	auto iter = m_dxMatrixMap.find(pNode->GetName());
	if (iter == m_dxMatrixMap.end())
	{
		m_dxMatrixMap[pNode->GetName()] = mat;
	}

	int iChild = pNode->GetChildCount();
	for (int iObj = 0; iObj < iChild; iObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(iObj);

		if (pChildNode->GetNodeAttribute() != NULL)
		{
			FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
			if (AttributeType != FbxNodeAttribute::eMesh &&
				AttributeType != FbxNodeAttribute::eSkeleton &&
				AttributeType != FbxNodeAttribute::eNull)
			{
				continue;
			}
		}
		PreProcess(pChildNode);
	}
}

void HFbxObj::ParseNode(FbxNode * pNode, FbxScene* pScene)
{
	shared_ptr<HObject> obj = make_shared<HObject>();
	obj->m_szName = to_mw(pNode->GetName());
}

void HFbxObj::ParseAnimation(FbxScene * pScene)
{
}

HFbxObj::HFbxObj()
{
	m_pFbxImporter = nullptr;
	m_pFbxScene = nullptr;
}

