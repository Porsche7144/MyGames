//#include "HFbxObj.h"
//FbxManager*	HFbxObj::g_pSDKManager = nullptr;
//
//bool HFbxObj::Load(std::string FileName)
//{
//	if (LoadFBX(FileName))
//	{
//		return true;
//	}
//
//	return false;
//}
//
//bool HFbxObj::LoadFBX(std::string FileName)
//{
//	if (Initialize(FileName) == false)
//	{
//		return false;
//	}
//
//	FbxNode* pFbxRootNode = m_pFbxScene->GetRootNode();
//	HStopwatch stopwatch;
//
//	PreProcess(pFbxRootNode);
//	for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
//	{
//		FbxNode* pNode = m_pFbxNodeList[iNode];
//		//shared_ptr<TObject> obj = make_shared<TObject>();	
//		HModelObj* obj = new HModelObj;
//		obj->m_szName = to_mw(pNode->GetName());
//		obj->m_pParentObj = m_hNodeMap[pNode->GetParent()];
//		m_hNodeMap[pNode] = obj;
//		m_hNodeList.push_back(obj);
//		if (pNode->GetMesh() != nullptr)
//		{
//			ParseMesh(pNode, pNode->GetMesh(), obj);
//		}
//	}
//#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
//	auto anim = m_pFbxScene->GetAnimationEvaluator();
//#else
//	auto anim = m_pFbxScene->GetEvaluator();
//#endif
//	ParseAnimation(m_pFbxScene);
//
//	float fCurrentTime = 0.0f;
//	while (fCurrentTime <= m_AnimScene.fEndTime)
//	{
//		FbxTime t;
//		t.SetSecondDouble(fCurrentTime);
//		for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
//		{
//			FbxNode* pNode = m_pFbxNodeList[iNode];
//			auto data = m_hNodeMap.find(pNode);
//			FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t);
//			AddKey(pNode, mat, fCurrentTime);
//
//			/*HAnimTrack track;
//			track.iTick = fCurrentTime * 30 * 160;
//			track.mat = DxConvertMatrix(ConvertMatrixA(mat));
//			data->second->m_AnimList.push_back(track);*/
//		}
//		fCurrentTime += m_AnimScene.fDeltaTime * 1;
//	}
//	stopwatch.Output(L"Load");
//
//	return true;
//}
//
//bool HFbxObj::Initialize(std::string FileName)
//{
//	// Manager 생성, IO세팅
//	if (g_pSDKManager == nullptr)
//	{
//		g_pSDKManager = FbxManager::Create();
//		if (g_pSDKManager == nullptr) return false;
//
//		FbxIOSettings* ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
//		if (ios == nullptr) return false;
//
//		g_pSDKManager->SetIOSettings(ios);
//	}
//
//	// Importer 생성
//	if (m_pFbxImporter == nullptr)
//	{
//		m_pFbxImporter = FbxImporter::Create(g_pSDKManager, "");
//		if (m_pFbxImporter == nullptr) return false;
//	}
//
//	// Scene 생성
//	if (m_pFbxScene == nullptr)
//	{
//		m_pFbxScene = FbxScene::Create(g_pSDKManager, "");
//		if (m_pFbxScene == nullptr) return false;
//	}
//
//	// 사용할 파일 이름과 IOSettings를 제공하여 임포터를 초기화
//	bool bRet = m_pFbxImporter->Initialize(FileName.c_str(), -1, g_pSDKManager->GetIOSettings());
//	if (bRet == false) return false;
//	
//	// Scene을 가져옴.
//	bRet = m_pFbxImporter->Import(m_pFbxScene);
//
//	// Import 이후에 세팅
//	FbxAxisSystem::MayaZUp.DeepConvertScene(m_pFbxScene);
//	FbxAxisSystem SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();
//
//	// 삼각형화
//	/*FbxGeometryConverter iGeomConverter(g_pSDKManager);
//	iGeomConverter.Triangulate(m_pFbxScene, true);*/
//
//	return true;
//}
//
//void HFbxObj::PreProcess(FbxNode * pNode)
//{
//	// 카메라와 라이트 정보는 불필요하므로 걸러냄.
//	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
//	{
//		return;
//	}
//
//	Matrix mat = Matrix::Identity;
//	m_pFbxNodeMap.insert(make_pair(pNode, m_MatrixList.size()));
//	m_MatrixList.push_back(mat);
//	m_pFbxNodeList.push_back(pNode);
//
//	int iChild = pNode->GetChildCount();
//	for (int iObj = 0; iObj < iChild; iObj++)
//	{
//		FbxNode* pChildNode = pNode->GetChild(iObj);
//
//		// 메쉬와 뼈대만 저장하도록.
//	/*	if (pChildNode->GetNodeAttribute() != NULL)
//		{
//			FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
//			if (AttributeType != FbxNodeAttribute::eMesh &&
//				AttributeType != FbxNodeAttribute::eSkeleton &&
//				AttributeType != FbxNodeAttribute::eNull)
//			{
//				continue;
//			}
//		}*/
//		// 갯수만큼 재귀호출.
//		PreProcess(pChildNode);
//	}
//}
//
//Matrix HFbxObj::ParseTransform(FbxNode* pNode, Matrix& matParentWorld)
//{
//	Matrix matWorld = Matrix::Identity;
//	return matWorld;
//}
//
//void HFbxObj::ReadTextureCoord(FbxMesh * pFbxMesh, FbxLayerElementUV * pUVSet, int vertexIndex, int uvIndex, FbxVector2 & uv)
//{
//	FbxLayerElementUV* pFbxLayerElementUV = pUVSet;
//
//	if (pFbxLayerElementUV == nullptr)
//	{
//		return;
//	}
//
//	switch (pFbxLayerElementUV->GetMappingMode())
//	{
//		case FbxLayerElementUV::eByControlPoint:
//		{
//			switch (pFbxLayerElementUV->GetReferenceMode())
//			{
//				case FbxLayerElementUV::eDirect:
//				{
//					FbxVector2 FbxUV = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
//					uv.mData[0] = FbxUV.mData[0];
//					uv.mData[1] = FbxUV.mData[1];
//					break;
//				}
//				case FbxLayerElementUV::eIndexToDirect:
//				{
//					int id = pFbxLayerElementUV->GetIndexArray().GetAt(vertexIndex);
//					FbxVector2 FbxUV = pFbxLayerElementUV->GetDirectArray().GetAt(id);
//					uv.mData[0] = FbxUV.mData[0];
//					uv.mData[1] = FbxUV.mData[1];
//					break;
//				}
//			}
//			break;
//		}
//
//		case FbxLayerElementUV::eByPolygonVertex:
//		{
//			switch (pFbxLayerElementUV->GetReferenceMode())
//			{
//				case FbxLayerElementUV::eDirect:
//				{
//
//				}
//				case FbxLayerElementUV::eIndexToDirect:
//				{
//					uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[0];
//					uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[1];
//					break;
//				}				
//			}
//			break;
//		}
//	}
//}
//
//std::string HFbxObj::ParseMaterial(FbxSurfaceMaterial * pMtrl)
//{
//	std::string name = pMtrl->GetName();
//
//	// sDiffuse 를 찾음.
//	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
//
//	if (Property.IsValid())
//	{
//		const FbxFileTexture* texture = Property.GetSrcObject<FbxFileTexture>(0);
//		if (texture != nullptr)
//		{
//			// sDiffuse 리턴
//			const char* szFilename = texture->GetFileName();
//			char drive[MAX_PATH];
//			char dir[MAX_PATH];
//			char name[MAX_PATH];
//			char ext[MAX_PATH];
//			_splitpath_s(szFilename, drive, dir, name, ext);
//			std::string texName = name;
//			
//
//			std::string Ext;
//			Ext = ext;
//
//			// FBX는 tga를 로드할 수 없다.
//			if (Ext == ".tga" || Ext == ".TGA")
//			{
//				Ext.clear();
//				Ext = ".dds";
//			}
//
//			texName += Ext;
//			return texName;
//		}
//	}
//
//	return std::string("");
//}
//
//void HFbxObj::ParseNode(FbxNode * pNode, Matrix matParent, HModelObj* pParentObj)
//{
//	if (pNode == nullptr) return;
//	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
//	{
//		return;
//	}
//
//	HModelObj* Obj = new HModelObj;
//	Obj->m_szName = to_mw(pNode->GetName());
//	Obj->m_pParentObj = pParentObj;
//	m_hNodeMap[pNode] = Obj;
//	m_hNodeList.push_back(Obj);
//	
//	Matrix matWorld = ParseTransform(pNode, matParent);
//
//	// World Matrix
//	Obj->m_matWorld = matWorld;
//	if (pNode->GetMesh() != nullptr)
//	{
//		// vb, ib
//		ParseMesh(pNode, pNode->GetMesh(), Obj);
//	}
//
//	int iChild = pNode->GetChildCount();
//	for (int iObj = 0; iObj < iChild; iObj++)
//	{
//		FbxNode* pChildNode = pNode->GetChild(iObj);
//		ParseNode(pChildNode, matWorld, Obj);
//	}
//}
//
//void HFbxObj::ParseMesh(FbxNode * pNode, FbxMesh * pMesh, HModelObj * pObj)
//{
//	std::vector<FbxLayerElementUV*> VertexUVSet;
//	std::vector<FbxLayerElementMaterial*> pMaterialSetList;
//	std::vector<FbxLayerElementVertexColor*> pVertexColor;
//
//	int iLayerCount = pMesh->GetLayerCount();
//	if (iLayerCount == 0 || pMesh->GetLayer(0)->GetNormals() == nullptr)
//	{
//		pMesh->InitNormals();
//#if (FBXSDK_VERSION_MAJOR >= 2015)
//		pMesh->GenerateNormals();
//#else
//		pMesh->ComputeVertexNormals();
//#endif
//	}
//
//	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
//	{
//		FbxLayer* pLayer = pMesh->GetLayer(iLayer);
//		
//		// 버텍스 컬러
//		if (pLayer->GetVertexColors() != NULL)
//		{
//			pVertexColor.push_back(pLayer->GetVertexColors());
//		}
//
//		// UV
//		if (pLayer->GetUVs() != NULL)
//		{
//			VertexUVSet.push_back(pLayer->GetUVs());
//		}
//		if (pMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
//		{
//			// 메쉬의 Material의 정보를 받아와 리스트에 넣는다.
//			pMaterialSetList.push_back(pMesh->GetLayer(iLayer)->GetMaterials());
//		}
//	}
//
//	// 폴리곤 수
//	int iPolyCount = pMesh->GetPolygonCount();
//	// 정점 수
//	int iVertexCount = pMesh->GetControlPointsCount();
//
//	// 트라이앵글 최대치
//	int iMaxTriangleCount = 0;
//	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
//	{
//		int iPolySize = pMesh->GetPolygonSize(iPoly);
//		iMaxTriangleCount += iPolySize - 2;
//	}
//
//	int iNumMtrl = pNode->GetMaterialCount();
//	// 받아온 Matrial의 갯수 만큼 리사이즈
//	if (iNumMtrl > 1)
//	{
//		pObj->m_SubMesh.resize(iNumMtrl);
//	}
//	else
//	{
//		pObj->m_SubMesh.resize(1);
//		pObj->m_SubMesh[0].m_VertexList.reserve(iMaxTriangleCount * 3);
//	}
//
//	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
//	{
//		FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(iMtrl);
//		if (pMtrl == nullptr)
//		{
//			continue;
//		}
//		pObj->FbxMaterialList.push_back(to_mw(ParseMaterial(pMtrl)));
//		pObj->m_SubMesh[iMtrl].m_VertexList.reserve(iMaxTriangleCount * 3);
//	}
//	   	 
//	// Transform
//
//	// FbxAMatrix A가 붙으면 그 자체로 역행렬
//	FbxAMatrix geom;
//	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
//	FbxVector4 rotation = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
//	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
//	geom.SetT(trans);
//	geom.SetR(rotation);
//	geom.SetS(scale);
//	// pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(geom));
//
//	/*geom = pNode->EvaluateGlobalTransform(1.0f);
//	pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(pNode->EvaluateGlobalTransform(1.0f)));*/
//
//	// 컬러 = 월드 * 역행렬 * 전치행렬
//	FbxAMatrix normalMatrix = geom;
//	normalMatrix = normalMatrix.Inverse();
//	normalMatrix = normalMatrix.Transpose();
//	
//	pObj->m_matWorld = DxConvertMatrix(ConvertMatrixA(pNode->EvaluateGlobalTransform(1.0f)));
//
//
//	FbxVector4* pVertexPositions = pMesh->GetControlPoints();
//
//	bool bSkinedMesh = ParseMeshSkinningMap(pMesh, pObj->m_WeightList, pObj);
//	pObj->m_bSkinnedMesh = bSkinedMesh;
//	//if (pObj->m_bSkinnedMesh == false)
//	//{
//	//	auto data = m_pFbxNodeMap.find(pNode);
//	//	int  iBoneIndex = data->second;
//	//	pObj->m_WeightList.resize(iVertexCount);
//	//	for (int iv = 0; iv < iVertexCount; iv++)
//	//	{
//	//		pObj->m_WeightList[iv].m_Index.push_back(iBoneIndex);
//	//		pObj->m_WeightList[iv].m_Weight.push_back(1.0f);
//	//	}
//	//}
//
//	int iBasePolyIndex = 0;
//
//	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
//	{
//		int iPolySize = pMesh->GetPolygonSize(iPoly);
//		int iTriangleCount = iPolySize - 2;
//
//		int iCornerIndices[3];
//		HTriangle Triangle;
//
//		// Sub Material
//		int iSubMtrl = 0;
//		if (pMaterialSetList.size() > 0)
//		{
//			// GetMappingMode() 요소가 표면에 매핑되는 방식을 얻어온다.
//			switch (pMaterialSetList[0]->GetMappingMode())
//			{
//				case FbxLayerElement::eByPolygon:
//				{
//					// GetReferenceMode() 좌표배열이 저장된 매핑정보를 얻어온다.
//					switch ((pMaterialSetList[0]->GetReferenceMode()))
//					{
//						case FbxLayerElement::eIndex:
//						{
//							iSubMtrl = iPoly;
//						}break;
//
//						// 인덱스 버퍼를 통한 참조
//						case FbxLayerElement::eIndexToDirect:
//						{
//							iSubMtrl = pMaterialSetList[0]->GetIndexArray().GetAt(iPoly);
//							// pObj->m_SubMesh[iSubMtrl].iCount++;
//						}break;
//					}
//				}
//				default:
//				{
//					break;
//				}
//			}
//			
//		}
//
//
//		for (int iTri = 0; iTri < iTriangleCount; iTri++)
//		{
//			int iVertIndex[3] = { 0, iTri + 2, iTri + 1 };
//
//			iCornerIndices[0] = pMesh->GetPolygonVertex(iPoly, iVertIndex[0]);
//			iCornerIndices[1] = pMesh->GetPolygonVertex(iPoly, iVertIndex[1]);
//			iCornerIndices[2] = pMesh->GetPolygonVertex(iPoly, iVertIndex[2]);
//
//			FbxVector4 vNormals[3];
//			ZeroMemory(vNormals, 3 * sizeof(FbxVector4));
//			int iPolyIndex = iPoly;
//
//			pMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[0], vNormals[0]);
//			pMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[1], vNormals[1]);
//			pMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[2], vNormals[2]);
//			
//			int u[3];
//			u[0] = pMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[0]);
//			u[1] = pMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[1]);
//			u[2] = pMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[2]);
//
//			for (int iIndex = 0; iIndex < 3; iIndex++)
//			{
//				PNCT_VERTEX v;
//				// geom에 pVertexPositions[iCornerIndices[iIndex]] 를 곱함.
//				auto finalPos = geom.MultT(pVertexPositions[iCornerIndices[iIndex]]);
//				v.p.x = finalPos.mData[0]; // x
//				v.p.y = finalPos.mData[2]; // z
//				v.p.z = finalPos.mData[1]; // y
//
//
//				// 컬러
//				FbxColor color = FbxColor(1, 1, 1, 1);
//				if (pVertexColor.size())
//				{
//					color = ReadColor(pMesh,
//						pVertexColor.size(),
//						pVertexColor[0],
//						iCornerIndices[iIndex],
//						iBasePolyIndex + iVertIndex[iIndex]);
//				}
//				v.c.x = (float)color.mRed;
//				v.c.y = (float)color.mGreen;
//				v.c.z = (float)color.mBlue;
//				v.c.w = 1;
//
//
//				// 노말
//				FbxVector4 normal = ReadNormal(pMesh, iCornerIndices[iIndex], iBasePolyIndex + iVertIndex[iIndex]);
//				finalPos = normalMatrix.MultT(normal);
//
//				v.n.x = finalPos.mData[0];
//				v.n.y = finalPos.mData[2];
//				v.n.z = finalPos.mData[1];
//
//				if (VertexUVSet.size())
//				{
//					for (int iUVIndex = 0; iUVIndex < 1; /*VertexUVSet.size();*/ ++iUVIndex)
//					{
//						FbxLayerElementUV* pUVSet = VertexUVSet[iUVIndex];
//						FbxVector2 uv(0, 0);
//						ReadTextureCoord(pMesh, pUVSet, iCornerIndices[iIndex], u[iIndex], uv);
//
//						v.t.x = uv.mData[0];
//						v.t.y = 1.0f - uv.mData[1];
//					}
//				}
//				
//				IW_VERTEX iw;
//				if (pObj->m_bSkinnedMesh)
//				{
//					HWeight* pW = &pObj->m_WeightList[iCornerIndices[iIndex]];
//
//					for (int i = 0; i < 4; i++)
//					{
//						iw.i1[i] = pW->m_Index[i];
//						iw.w1[i] = pW->m_Weight[i];
//					}
//				}
//				else
//				{
//					auto data = m_pFbxNodeMap.find(pNode);
//					iw.i1[0] = data->second; // 자기 자신
//					iw.w1[0] = 1.0f;
//					iw.w1[1] = 0.0f;
//					iw.w1[2] = 0.0f;
//					iw.w1[3] = 0.0f;
//				}
//
//				Triangle.vVertex[iIndex] = v;
//				Triangle.vIWVertex[iIndex] = iw;
//			}
//			if (iNumMtrl > 1)
//			{
//				pObj->m_SubMesh[iSubMtrl].SetUniqueBuffer(Triangle);
//			}
//			else
//			{
//				pObj->m_SubMesh[0].SetUniqueBuffer(Triangle);
//			}
//		}
//		iBasePolyIndex += iPolySize;
//	}
//
//}
//
//FbxVector4 HFbxObj::ReadNormal(const FbxMesh* mesh,	int controlPointIndex, int VertexCounter)
//{
//	if (mesh->GetElementNormalCount() < 1) {}
//
//	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
//	// 노말 획득 
//	FbxVector4 result;
//	// 노말 벡터를 저장할 벡터 
//	switch (vertexNormal->GetMappingMode()) 	// 매핑 모드 
//	{
//		// 제어점 마다 1개의 매핑 좌표가 있다.
//	case FbxGeometryElement::eByControlPoint:
//	{
//		// control point mapping 
//		switch (vertexNormal->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
//			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
//			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
//		} break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
//			// 인덱스를 얻어온다. 
//			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
//			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
//			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
//		}break;
//		}break;
//	}break;
//	// 정점 마다 1개의 매핑 좌표가 있다.
//	case FbxGeometryElement::eByPolygonVertex:
//	{
//		switch (vertexNormal->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(VertexCounter).mData[0]);
//			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(VertexCounter).mData[1]);
//			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(VertexCounter).mData[2]);
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexNormal->GetIndexArray().GetAt(VertexCounter);
//			// 인덱스를 얻어온다. 
//			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
//			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
//			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
//		}break;
//		}
//	}break;
//	}
//	return result;
//}
//
//FbxColor HFbxObj::ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet,
//	DWORD dwDCCIndex, DWORD dwVertexIndex)
//{
//	FbxColor Value(1, 1, 1, 1);
//	if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
//	{
//		// Crack apart the FBX dereferencing system for Color coordinates		
//		switch (pVertexColorSet->GetMappingMode())
//		{
//			case FbxLayerElement::eByControlPoint:
//				switch (pVertexColorSet->GetReferenceMode())
//				{
//					case FbxLayerElement::eDirect:
//					{
//						Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
//					}break;
//
//					case FbxLayerElement::eIndexToDirect:
//					{
//						int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
//						Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
//					}break;
//				}
//
//			case FbxLayerElement::eByPolygonVertex:
//				switch (pVertexColorSet->GetReferenceMode())
//				{
//					case FbxLayerElement::eDirect:
//					{
//						int iColorIndex = dwVertexIndex;
//						Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
//					}break;
//
//					case FbxLayerElement::eIndexToDirect:
//					{
//						int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
//						Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
//					}break;
//				}
//			break;
//		}
//	}
//	return Value;
//}
//
//bool HFbxObj::CreateInputLayOut()
//{
//	HRESULT hr = S_OK;
//
//	// LayOut
//	const D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		//{ "Transform",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
//		//{ "Transform",1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
//		//{ "Transform",2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
//		//{ "Transform",3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
//	};
//
//	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
//	hr = g_pd3dDevice->CreateInputLayout(layout, iNumElement, pVSObj->GetBufferPointer(),
//		pVSObj->GetBufferSize(), &m_pInputLayout);
//	if (FAILED(hr))
//	{
//		return false;
//	}
//
//	return true;
//}
//
//HFbxObj::HFbxObj()
//{
//	m_pFbxImporter = nullptr;
//	m_pFbxScene = nullptr;
//}