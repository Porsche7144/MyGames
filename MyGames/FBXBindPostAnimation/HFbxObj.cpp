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

	// Import ���Ŀ� ����
	FbxAxisSystem::MayaZUp.DeepConvertScene(m_pFbxScene);
	FbxAxisSystem SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();

	// �ﰢ��ȭ
	/*FbxGeometryConverter iGeomConverter(g_pSDKManager);
	iGeomConverter.Triangulate(m_pFbxScene, true);*/

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
		m_pNodeMap[pNode] = m_MatrixList.size();
		m_MatrixList.push_back(mat);
	}

	int iChild = pNode->GetChildCount();
	for (int iObj = 0; iObj < iChild; iObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(iObj);

		// �޽��� ���븸 �����ϵ���.
	/*	if (pChildNode->GetNodeAttribute() != NULL)
		{
			FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
			if (AttributeType != FbxNodeAttribute::eMesh &&
				AttributeType != FbxNodeAttribute::eSkeleton &&
				AttributeType != FbxNodeAttribute::eNull)
			{
				continue;
			}
		}*/
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
			

			std::string Ext;
			Ext = ext;

			// FBX�� tga�� �ε��� �� ����.
			if (Ext == ".tga" || Ext == ".TGA")
			{
				Ext.clear();
				Ext = ".dds";
			}

			texName += Ext;
			return texName;
		}
	}

	return std::string("");
}

void HFbxObj::ParseNode(FbxNode * pNode, Matrix matParent)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}

	HModelObj* Obj = new HModelObj;
	Obj->m_szName = to_mw(pNode->GetName());
	m_hNodeMap[pNode] = Obj;
	m_hNodeList.push_back(Obj);
	
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

void HFbxObj::ParseMesh(FbxNode * pNode, FbxMesh * pMesh, HModelObj * pObj)
{
	std::vector<FbxLayerElementUV*> VertexUVSet;
	std::vector<FbxLayerElementMaterial*> pMaterialSetList;
	std::vector<FbxLayerElementVertexColor*> pVertexColor;

	int iLayerCount = pMesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pMesh->GetLayer(iLayer);
		
		// ���ؽ� �÷�
		if (pLayer->GetVertexColors() != NULL)
		{
			pVertexColor.push_back(pLayer->GetVertexColors());
		}

		// UV
		if (pLayer->GetUVs() != NULL)
		{
			VertexUVSet.push_back(pLayer->GetUVs());
		}
		if (pMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
		{
			// �޽��� Material�� ������ �޾ƿ� ����Ʈ�� �ִ´�.
			pMaterialSetList.push_back(pMesh->GetLayer(iLayer)->GetMaterials());
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

	// �޾ƿ� Matrial�� ���� ��ŭ ��������
	if (iNumMtrl > 1)
	{
		pObj->m_SubMesh.resize(iNumMtrl);
	}
	
	// Transform

	// FbxAMatrix A�� ������ �� ��ü�� �����
	FbxAMatrix geom;
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rotation = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rotation);
	geom.SetS(scale);
	// pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(geom));

	/*geom = pNode->EvaluateGlobalTransform(1.0f);
	pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(pNode->EvaluateGlobalTransform(1.0f)));*/

	// �÷� = ���� * ����� * ��ġ���
	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(pNode->EvaluateGlobalTransform(1.0f)));

	// ������ ��
	int iPolyCount = pMesh->GetPolygonCount();
	// ���� ��
	int iVertexCount = pMesh->GetControlPointsCount();
	FbxVector4* pVertexPositions = pMesh->GetControlPoints();

	bool bSkinedMesh = ParseMeshSkinningMap(pMesh, pObj->m_WeightList);
	pObj->m_bSkinnedMesh = bSkinedMesh;
	if (pObj->m_bSkinnedMesh == false)
	{
		auto data = m_pNodeMap.find(pNode);
		int  iBoneIndex = data->second;
		pObj->m_WeightList.resize(iVertexCount);
		for (int iv = 0; iv < iVertexCount; iv++)
		{
			pObj->m_WeightList[iv].m_Index.push_back(iBoneIndex);
			pObj->m_WeightList[iv].m_Weight.push_back(1.0f);
		}
	}

	int iBasePolyIndex = 0;

	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;

		int iCornerIndices[3];
		HTriangle Triangle;

		// Sub Material
		int iSubMtrl = 0;
		if (pMaterialSetList.size() > 0)
		{
			// GetMappingMode() ��Ұ� ǥ�鿡 ���εǴ� ����� ���´�.
			switch (pMaterialSetList[0]->GetMappingMode())
			{
				case FbxLayerElement::eByPolygon:
				{
					// GetReferenceMode() ��ǥ�迭�� ����� ���������� ���´�.
					switch ((pMaterialSetList[0]->GetReferenceMode()))
					{
						case FbxLayerElement::eIndex:
						{
							iSubMtrl = iPoly;
						}break;

						// �ε��� ���۸� ���� ����
						case FbxLayerElement::eIndexToDirect:
						{
							iSubMtrl = pMaterialSetList[0]->GetIndexArray().GetAt(iPoly);
							// pObj->m_SubMesh[iSubMtrl].iCount++;
						}break;
					}

					default:
					{
						break;
					}

				}
			}
			
		}


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
				v.p.x = finalPos.mData[0]; // x
				v.p.y = finalPos.mData[2]; // z
				v.p.z = finalPos.mData[1]; // y


				// �÷�
				FbxColor color = FbxColor(1, 1, 1, 1);
				if (pVertexColor.size())
				{
					color = ReadColor(pMesh,
						pVertexColor.size(),
						pVertexColor[0],
						iCornerIndices[iIndex],
						iBasePolyIndex + iVertIndex[iIndex]);
				}
				v.c.x = (float)color.mRed;
				v.c.y = (float)color.mGreen;
				v.c.z = (float)color.mBlue;
				v.c.w = 1;


				// �븻
				FbxVector4 normal = ReadNormal(pMesh, iCornerIndices[iIndex], iBasePolyIndex + iVertIndex[iIndex]);
				finalPos = normalMatrix.MultT(normal);

				v.n.x = finalPos.mData[0];
				v.n.y = finalPos.mData[2];
				v.n.z = finalPos.mData[1];

				if (VertexUVSet.size())
				{
					for (int iUVIndex = 0; iUVIndex < 1; /*VertexUVSet.size();*/ ++iUVIndex)
					{
						FbxLayerElementUV* pUVSet = VertexUVSet[iUVIndex];
						FbxVector2 uv(0, 0);
						ReadTextureCoord(pMesh, pUVSet, iCornerIndices[iIndex], u[iIndex], uv);

						v.t.x = uv.mData[0];
						v.t.y = 1.0f - uv.mData[1];
					}
				}
				
				IW_VERTEX iw;
				if (pObj->m_bSkinnedMesh)
				{
					HWeight* pW = &pObj->m_WeightList[iCornerIndices[iIndex]];

					int iMax = -99;
					int iWeightSize = pW->m_Weight.size();

					if (pW != nullptr)
					{
						if (iMax < iWeightSize)
						{
							iMax = pW->m_Weight.size();
						}
					}

					for (int i = 0; i < pW->m_Index.size(); i++)
					{
						if (i < 4)
							iw.i1[i] = pW->m_Index[i];
						else
							iw.i2[i - 4] = pW->m_Index[i];

					}
					for (int i = 0; i < pW->m_Weight.size(); i++)
					{
						if (i < 4)
							iw.w1[i] = pW->m_Weight[i];
						else
							iw.w2[i - 4] = pW->m_Weight[i];
					}
				}
				else
				{
					iw.i1[0] = 0; // �ڱ� �ڽ�
					iw.w1[0] = 1.0f;
				}

				Triangle.vVertex[iIndex] = v;
				Triangle.vIWVertex[iIndex] = iw;
			}
			if (iNumMtrl > 1)
			{
				pObj->m_SubMesh[iSubMtrl].m_TriangleList.push_back(Triangle);
			}
			else
			{
				pObj->m_TriangleList.push_back(Triangle);
			}
		}
		iBasePolyIndex += iPolySize;
	}

}

FbxVector4 HFbxObj::ReadNormal(const FbxMesh* mesh,	int controlPointIndex, int VertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	// �븻 ȹ�� 
	FbxVector4 result;
	// �븻 ���͸� ������ ���� 
	switch (vertexNormal->GetMappingMode()) 	// ���� ��� 
	{
		// ������ ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// ���� ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(VertexCounter).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(VertexCounter).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(VertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(VertexCounter);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}

FbxColor HFbxObj::ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet,
	DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
	{
		// Crack apart the FBX dereferencing system for Color coordinates		
		switch (pVertexColorSet->GetMappingMode())
		{
			case FbxLayerElement::eByControlPoint:
				switch (pVertexColorSet->GetReferenceMode())
				{
					case FbxLayerElement::eDirect:
					{
						Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
					}break;

					case FbxLayerElement::eIndexToDirect:
					{
						int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
						Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
					}break;
				}

			case FbxLayerElement::eByPolygonVertex:
				switch (pVertexColorSet->GetReferenceMode())
				{
					case FbxLayerElement::eDirect:
					{
						int iColorIndex = dwVertexIndex;
						Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
					}break;

					case FbxLayerElement::eIndexToDirect:
					{
						int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
						Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
					}break;
				}
			break;
		}
	}
	return Value;
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
	// if (m_pFbxRootNode != nullptr) { m_pFbxRootNode->Destroy(); }
	if (g_pSDKManager) { g_pSDKManager->Destroy(); }
}