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

	m_pFbxRootNode = m_pFbxScene->GetRootNode();
	PreProcess(m_pFbxRootNode);
	ParseNode(m_pFbxRootNode, Matrix::Identity);
	ParseAnimation(m_pFbxScene);

	return true;
}

bool HFbxObj::Initialize(std::string FileName)
{
	// Manager ����, IO����
	if (g_pSDKManager == nullptr)
	{
		g_pSDKManager = FbxManager::Create();
		if (g_pSDKManager == nullptr) return false;

		FbxIOSettings* ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
		if (ios == nullptr) return false;

		g_pSDKManager->SetIOSettings(ios);
	}

	// Importer ����
	if (m_pFbxImporter == nullptr)
	{
		m_pFbxImporter = FbxImporter::Create(g_pSDKManager, "");
		if (m_pFbxImporter == nullptr) return false;
	}

	// Scene ����
	if (m_pFbxScene == nullptr)
	{
		m_pFbxScene = FbxScene::Create(g_pSDKManager, "");
		if (m_pFbxScene == nullptr) return false;
	}

	// ����� ���� �̸��� IOSettings�� �����Ͽ� �����͸� �ʱ�ȭ
	bool bRet = m_pFbxImporter->Initialize(FileName.c_str(), -1, g_pSDKManager->GetIOSettings());
	if (bRet == false) return false;
	
	// Scene�� ������.
	bRet = m_pFbxImporter->Import(m_pFbxScene);

	// �ﰢ��ȭ
	FbxGeometryConverter iGeomConverter(g_pSDKManager);
	iGeomConverter.Triangulate(m_pFbxScene, true);

	return true;
}

void HFbxObj::PreProcess(FbxNode * pNode)
{
	// ī�޶�� ����Ʈ ������ ���ʿ��ϹǷ� �ɷ���.
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
}

Matrix HFbxObj::ParseTransform(FbxNode* pNode, Matrix& matParentWorld)
{
	Matrix matWorld = Matrix::Identity;
	return matWorld;
}

void HFbxObj::ReadTextureCoord(FbxMesh * pFbxMesh, FbxLayerElementUV * pUVSet, int vertexIndex, int uvIndex, FbxVector2 & uv)
{
	FbxLayerElementUV* pFbxLayerElementUV = pUVSet;

	if (pFbxLayerElementUV == nullptr)
	{
		return;
	}

	switch (pFbxLayerElementUV->GetMappingMode())
	{
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pFbxLayerElementUV->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				{
					FbxVector2 FbxUV = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
					uv.mData[0] = FbxUV.mData[0];
					uv.mData[1] = FbxUV.mData[1];
					break;
				}
				case FbxLayerElementUV::eIndexToDirect:
				{
					int id = pFbxLayerElementUV->GetIndexArray().GetAt(vertexIndex);
					FbxVector2 FbxUV = pFbxLayerElementUV->GetDirectArray().GetAt(id);
					uv.mData[0] = FbxUV.mData[0];
					uv.mData[1] = FbxUV.mData[1];
					break;
				}
			}
			break;
		}

		case FbxLayerElementUV::eByPolygonVertex:
		{
			switch (pFbxLayerElementUV->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				{

				}
				case FbxLayerElementUV::eIndexToDirect:
				{
					uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[0];
					uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[1];
					break;
				}				
			}
			break;
		}
	}
}

std::string HFbxObj::ParseMaterial(FbxSurfaceMaterial * pMtrl)
{
	std::string name = pMtrl->GetName();

	// sDiffuse �� ã��.
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);

	if (Property.IsValid())
	{
		const FbxFileTexture* texture = Property.GetSrcObject<FbxFileTexture>(0);
		if (texture != nullptr)
		{
			// sDiffuse ����
			const char* szFilename = texture->GetFileName();
			char drive[MAX_PATH];
			char dir[MAX_PATH];
			char name[MAX_PATH];
			char ext[MAX_PATH];
			_splitpath_s(szFilename, drive, dir, name, ext);
			std::string texName = name;
			texName += ext;

			return texName;
		}
	}

	return std::string("");
}

void HFbxObj::ParseNode(FbxNode * pNode, Matrix matParent)
{
	HModelObject* Obj = new HModelObject;
	Obj->m_szName = to_mw(pNode->GetName());
	m_hMeshMap[pNode] = Obj;
	
	Matrix matWorld = ParseTransform(pNode, matParent);

	// World Matrix
	Obj->m_matWorld = matWorld;
	if (pNode->GetMesh() != nullptr)
	{
		// vb, ib
		ParseMesh(pNode, pNode->GetMesh(), Obj);
	}

	int iChild = pNode->GetChildCount();
	for (int iObj = 0; iObj < iChild; iObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(iObj);
		ParseNode(pChildNode, matWorld);
	}
}

void HFbxObj::ParseAnimation(FbxScene * pScene)
{
}

void HFbxObj::ParseMesh(FbxNode * pNode, FbxMesh * pMesh, HModelObject * pObj)
{
	std::vector<FbxLayerElementUV*> VertexUVSet;
	int iLayerCount = pMesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pMesh->GetLayer(iLayer);
		
		// ���ؽ� �÷�
		if (pLayer->GetVertexColors() != NULL)
		{

		}

		// UV
		if (pLayer->GetUVs() != NULL)
		{
			VertexUVSet.push_back(pLayer->GetUVs());
		}
	}

	std::vector<std::string> FbxMaterialList;
	int iNumMtrl = pNode->GetMaterialCount();
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(iMtrl);
		if (pMtrl == nullptr)
		{
			continue;
		}
		pObj->FbxMaterialList.push_back(to_mw(ParseMaterial(pMtrl)));
	}
	
	// Transform

	// FbxAMatrix A�� ������ �� ��ü�� �����
	FbxAMatrix geom;
	//FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	//FbxVector4 rotation = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	//FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	//geom.SetT(trans);
	//geom.SetR(rotation);
	//geom.SetS(scale);
	//pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(geom));

	geom = pNode->EvaluateGlobalTransform(1.0f);
	pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(pNode->EvaluateGlobalTransform(1.0f)));

	// ������ ��
	int iPolyCount = pMesh->GetPolygonCount();
	// ���� ��
	int iVertexCount = pMesh->GetControlPointsCount();
	FbxVector4* pVertexPositions = pMesh->GetControlPoints();

	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;

		int iCornerIndices[3];
		HTriangle Triangle;
		for (int iTri = 0; iTri < iTriangleCount; iTri++)
		{
			int iVertIndex[3] = { 0, iTri + 2, iTri + 1 };

			iCornerIndices[0] = pMesh->GetPolygonVertex(iPoly, iVertIndex[0]);
			iCornerIndices[1] = pMesh->GetPolygonVertex(iPoly, iVertIndex[1]);
			iCornerIndices[2] = pMesh->GetPolygonVertex(iPoly, iVertIndex[2]);

			FbxVector4 vNormals[3];
			ZeroMemory(vNormals, 3 * sizeof(FbxVector4));
			int iPolyIndex = iPoly;

			pMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[0], vNormals[0]);
			pMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[1], vNormals[1]);
			pMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[2], vNormals[2]);
			
			int u[3];
			u[0] = pMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[0]);
			u[1] = pMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[1]);
			u[2] = pMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[2]);

			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				PNCT_VERTEX v;
				// geom�� pVertexPositions[iCornerIndices[iIndex]] �� ����.
				auto finalPos = geom.MultT(pVertexPositions[iCornerIndices[iIndex]]);

				v.p.x = pVertexPositions[iCornerIndices[iIndex]].mData[0]; // x
				v.p.y = pVertexPositions[iCornerIndices[iIndex]].mData[2]; // z
				v.p.z = pVertexPositions[iCornerIndices[iIndex]].mData[1]; // y

				v.c = Vector4(1, 1, 0, 1);

				v.n.x = vNormals[iCornerIndices[iIndex]].mData[0];
				v.n.y = vNormals[iCornerIndices[iIndex]].mData[2];
				v.n.z = vNormals[iCornerIndices[iIndex]].mData[1];

				for (int iUVIndex = 0; iUVIndex < VertexUVSet.size(); ++iUVIndex)
				{
					FbxLayerElementUV* pUVSet = VertexUVSet[iUVIndex];
					FbxVector2 uv(0, 0);
					ReadTextureCoord(pMesh, pUVSet, iCornerIndices[iIndex], u[iIndex], uv);

					v.t.x = uv.mData[0];
					v.t.y = 1.0f - uv.mData[1];
				}

				Triangle.vVertex[iIndex] = v;
			}
			pObj->m_TriangleList.push_back(Triangle);
		}
	}

}

HFbxObj::HFbxObj()
{
	m_pFbxImporter = nullptr;
	m_pFbxScene = nullptr;
}

HFbxObj::~HFbxObj()
{
	if (m_pFbxImporter) { m_pFbxImporter->Destroy(); }
	if (m_pFbxScene) { m_pFbxScene->Destroy(); }
	// if (m_pFbxRootNode) { m_pFbxRootNode->Destroy(); }
	if (g_pSDKManager) { g_pSDKManager->Destroy(); }
}

