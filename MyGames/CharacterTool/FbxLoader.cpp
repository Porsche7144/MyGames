#include "FbxObj.h"
FbxManager* FbxObj::g_pSDKManager = nullptr;

FbxAMatrix FbxObj::GetGeometryTransformation(FbxNode* pNode)
{
	if (!pNode)
	{
		throw std::exception("Null for Mesh Geometry");
	}

	FbxVector4 IT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 IR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 IS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(IT, IR, IS);
}

Matrix FbxObj::ParseTransform(FbxNode* pNode, const Matrix& matParentWorld)
{
	Matrix matWorld;
	Matrix matLocal;
	Matrix matGeom;

	bool bTransform = true;

	FbxMatrix geom = GetGeometryTransformation(pNode);

	if (m_BindPoseMap.empty())
	{
		BindPoseMap::iterator iter = m_BindPoseMap.find(pNode);
		if (iter != m_BindPoseMap.end())
		{
			FbxMatrix pose = iter->second;
			matWorld = DxConvertMatrix(ConvertMatrixA(pose));
			matWorld = DxConvertMatrix(ConvertMatrixA(geom));
			bTransform = false;
		}
	}

	if (bTransform)
	{
		FbxVector4 Translation;
		if (pNode->LclTranslation.IsValid())
		{
			Translation = pNode->LclTranslation.Get();
		}

		FbxVector4 Rotation;
		if (pNode->LclRotation.IsValid())
		{
			Rotation = pNode->LclRotation.Get();
		}

		FbxVector4 Scale;
		if (pNode->LclScaling.IsValid())
		{
			Scale = pNode->LclScaling.Get();
		}

		FbxMatrix matTransform(Translation, Rotation, Scale);
		matGeom = DxConvertMatrix(ConvertMatrixA(geom));
		matLocal = DxConvertMatrix(ConvertMatrixA(matTransform));
		matWorld = matLocal * matParentWorld;
	}

	return matWorld;

}

bool FbxObj::PreProcess(FbxNode* pNode)
{
	// ī�޶�� ����Ʈ ������ ���ʿ��ϹǷ� �ɷ���.
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return false;
	}

	Matrix mat = Matrix::Identity;
	MatrixMap::iterator iter = m_matFrameMatrixMap.find(to_mw(pNode->GetName()));
	if (iter == m_matFrameMatrixMap.end())
	{
		m_matFrameMatrixMap[to_mw(pNode->GetName())] = mat;
	}

	int iChild = pNode->GetChildCount();
	for (int iObj = 0; iObj < iChild; iObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(iObj);

		// �޽��� ���븸 �����ϵ���.
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

		// ������ŭ ���ȣ��.
		PreProcess(pChildNode);
	}

	return true;
}

void FbxObj::ParseMesh(FbxNode* pNode, FbxMesh* pFbxMesh, Mesh pMesh, FbxData pData)
{
	std::vector<FbxMaterial*> fbxMaterialList;

	if (pFbxMesh)
	{
		DWORD dwLayerCount = pFbxMesh->GetLayerCount();
		DWORD dwVertexColor = 0;
		FbxLayerElementVertexColor* pVertexColorSet = NULL;
		DWORD dwUVSetCount = 0;
		std::vector<FbxLayerElementUV*> VertexUVSet;
		FbxLayerElementMaterial* pMaterialSet = NULL;

		for (DWORD dwLayerIndex = 0; dwLayerIndex < dwLayerCount; ++dwLayerIndex)
		{
			FbxLayer* pLayer = pFbxMesh->GetLayer(dwLayerIndex);
			if (pLayer->GetVertexColors == 0)
			{
				dwVertexColor++;
				pVertexColorSet = pLayer->GetVertexColors();
			}
			if (pLayer->GetUVs() != NULL)
			{
				dwUVSetCount++;
				VertexUVSet.push_back(pLayer->GetUVs());
			}
			if (pFbxMesh->GetLayer(dwLayerIndex)->GetMaterials() != NULL)
			{
				// 2�� �̻��� ���̾�� ������� ����.
				pMaterialSet = pLayer->GetMaterials();
				
				int iMaterialCount = pMaterialSet->mDirectArray->GetCount();
				//if (iMaterialCount > 1)
				//{
				//	for (DWORD i = 0; i < iMaterialCount; ++i)
				//	{
				//		FbxMaterial* pMaterial;
				//	}
				//}
			}

		}

	}

}
bool FbxObj::ParseNode(FbxNode* pNode, FbxScene* pScene, Mesh* pParent, const Matrix& matParentWorld)
{
	if (pNode == nullptr)
	{
		return false;
	}
	if (pNode->GetCamera() || pNode->GetLight())
	{
		return false;
	}
	
	shared_ptr<FbxData> pData = make_shared<FbxData>();
	shared_ptr<Mesh> pMesh = make_shared<Mesh>();
	pMesh->m_szName = to_mw(pNode->GetName());

	// �ּ�ó���ص� ����. ����׿뵵
	Matrix matWorld = ParseTransform(pNode, matParentWorld);

	FbxMatrix geom = GetGeometryTransformation(pNode);
	Matrix matGeom = DxConvertMatrix(ConvertMatrixA(geom));
	
	pMesh->m_matXFormToWorld = matGeom;
	pMesh->m_iMtrlRef = -1;
	pData->m_iNumFace = 0;
	pMesh->m_iNumFace = 0;

	m_pMesh.push_back(pMesh);
	m_pMeshData.push_back(pData);

	m_MeshMap[pNode] = pMesh.get();

	if (pNode->GetMesh() != NULL)
	{
		
	}

	int childCount = pNode->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		FbxNode* pFbxChildNode = pNode->GetChild(i);
		ParseNode(pFbxChildNode, pScene, pMesh.get(), matWorld);
	}

	return true;
}

bool FbxObj::Load(ID3D11Device* pDevice, const char* fbxFileName)
{

	if (!LoadFBX(pDevice, fbxFileName))
	{
		return false;
	}

	FbxNode* pFbxRootNode = m_FbxScene->GetRootNode();
	assert(m_FbxScene->GetRootNode() != NULL);

	Matrix matIdentity = Matrix::Identity;

	PreProcess(pFbxRootNode);
	ParseNode(pFbxRootNode, m_FbxScene, NULL, matIdentity);

	return true;
}

bool FbxObj::LoadFBX(ID3D11Device* pDevice, const char* fbxFileName)
{
	if (g_pSDKManager == nullptr)
	{
		g_pSDKManager = FbxManager::Create();
		if (!g_pSDKManager)
		{
			return false;
		}

		auto ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
		if (!ios)
		{
			return false;
		}

		g_pSDKManager->SetIOSettings(ios);
	}

	if (!m_FbxImporter)
	{
		m_FbxImporter = FbxImporter::Create(g_pSDKManager, "");
		if (!m_FbxImporter)
		{
			return false;
		}
	}

	m_FbxScene = FbxScene::Create(g_pSDKManager, "");
	if (!m_FbxScene)
	{
		return false;
	}

	// ���� ��� ���� FBX SDK�� FBX SDK 2011.2, FBX SDK 2011.1, FBX SDK 2010.2, FBX SDK 2010.0, FBX SDK 2009 �� 
	// ���� ���� ������ �ִ�.
	// FBX ���� ���Ŀ��� ���� ��ȣ�� �ִ�. FBX SDK 2010.2�� FBX ���� ���� 7.1(�⺻��), 7.0 �� 6.1�� �������� FBX ���� ���� 
	// 7.1, 7.0, 6.1 �� 6.0�� �����´�(�������� �� ������ ���� ���� ����).
	// FBX ���Ͽ��� ����ϴ� ���� ���� ������ ���� ���α׷��� ����� FBX SDK �������� �����Ǵ��� ���θ� Ȯ���� �� �ִ�.
	int pMajorVersion; int pMinorVersion; int pRevision;
	g_pSDKManager->GetFileFormatVersion(pMajorVersion, pMinorVersion, pRevision);

	// ����� ���� �̸��� IOSettings�� �����Ͽ� �����͸� �ʱ�ȭ
	bool bRet = m_FbxImporter->Initialize(fbxFileName, -1, g_pSDKManager->GetIOSettings());
	if (bRet == false)
	{
		return false;
	}
	
	// Scene�� ������.
	bRet = m_FbxImporter->Import(m_FbxScene);
	if (bRet == false)
	{
		return false;
	}

	// Import ���Ŀ� ����
	FbxAxisSystem::MayaZUp.DeepConvertScene(m_FbxScene);
	FbxAxisSystem SceneAxisSystem = m_FbxScene->GetGlobalSettings().GetAxisSystem();

	return true;
}

FbxObj::FbxObj()
{
}

FbxObj::~FbxObj()
{
}
