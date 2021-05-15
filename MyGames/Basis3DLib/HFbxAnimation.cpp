//#include "HFbxObj.h"
//
//void HFbxObj::AddKey(FbxNode* pNode, FbxMatrix matGlobal, float CurrentTime)
//{
//	auto anim = m_pFbxScene->GetAnimationEvaluator();
//	auto data = m_hNodeMap.find(pNode);
//
//	// matGlobal = �ڽ� * �θ�
//	Matrix pChildGlobal = DxConvertMatrix(
//		ConvertMatrixA(matGlobal));
//	FbxNode* pParentNode = pNode->GetParent();
//
//	Matrix matInvParentGlobal = Matrix::Identity;
//	if (pParentNode)
//	{
//		FbxTime t;
//		t.SetSecondDouble(CurrentTime);
//		FbxAMatrix matFbxParent =
//			anim->GetNodeGlobalTransform(pParentNode, t);
//		matInvParentGlobal = DxConvertMatrix(
//			ConvertMatrixA(matFbxParent));
//		matInvParentGlobal = matInvParentGlobal.Invert();
//	}
//	Matrix m = pChildGlobal * matInvParentGlobal;
//
//	// ����
//	Vector3 scale;
//	Quaternion qRot;
//	Vector3 pos;
//	if (m.Decompose(scale, qRot, pos))
//	{
//		HAnimTrack track;
//		track.iTick = CurrentTime * 30 * 160;
//		track.s = scale;
//		track.q = qRot;
//		track.p = pos;
//		track.mat = pChildGlobal;
//		data->second->m_AnimList.push_back(track);
//	}
//	else
//	{
//		int k = 0;
//	}
//}
//
//bool HFbxObj::ParseMeshSkinningMap(const FbxMesh * pMesh, std::vector<HWeight>& skindata, HModelObj* pObj)
//{
//	// ��Ų�� ����
//	int iDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
//	if (iDeformerCount == 0)
//	{
//		return false;
//	}
//
//	// ������ ����
//	int iVertexCount = pMesh->GetControlPointsCount();
//	skindata.resize(iVertexCount);
//
//	for (int iDeformerIndex = 0; iDeformerIndex < iDeformerCount; iDeformerIndex++)
//	{
//		auto pSkin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(iDeformerIndex, FbxDeformer::eSkin));
//
//		int iClusterCount = pSkin->GetClusterCount();
//
//		for (int iClusterIndex = 0; iClusterIndex < iClusterCount; iClusterIndex++)
//		{
//			auto pCluster = pSkin->GetCluster(iClusterIndex);
//
//			////////////////////////////////////////////////////////////
//
//			FbxAMatrix matXBindPos;
//			FbxAMatrix matReferenceGlobalInitPosition;
//
//			// �ش� ���� ���� Matrix�� ����.
//			pCluster->GetTransformLinkMatrix(matXBindPos); 
//			// �θ���� ���� �� Matrix
//			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
//
//			// �θ����(matReferenceGlobalInitPosition)�� ������� ����
//			// �ڱ��ڽ� ������ ���� ��İ��� ������.
//			FbxMatrix matBindPos = matReferenceGlobalInitPosition.Inverse() * matXBindPos;
//						
//			Matrix matInvBindPos = DxConvertMatrix(ConvertMatrixA(matBindPos));
//			// ���� -> ���� ��ȯ�� ���� �����.
//			matInvBindPos = matInvBindPos.Invert();
//
//
//			// �ش��ϴ� ��Ų�� �̸��� ��� ����
//			std::string szName = pCluster->GetLink()->GetName();
//			m_dxMatrixMap.insert(make_pair(szName, matInvBindPos));
//			pObj->m_dxMatrixBindPoseMap.insert(make_pair(szName, matInvBindPos));
//
//			int iClusterSize = pCluster->GetControlPointIndicesCount();
//			auto data = m_pFbxNodeMap.find(pCluster->GetLink());
//
//			// skindata�� ����� ��� ���� �� ��.
//			// �ε��� ������ �ƴ�.
//			int iBoneIndex = data->second;
//
//			// Cluster�� ������ �޴� ������ �ε���
//			int* pIndeces = pCluster->GetControlPointIndices();
//			double* pWeights = pCluster->GetControlPointWeights();
//
//			for (int i = 0; i < iClusterSize; i++)
//			{
//				skindata[pIndeces[i]].InsertWeight(iBoneIndex, pWeights[i]);
//			}
//
//		}
//
//	}
//
//	return true;
//}
//
//void HFbxObj::ParseAnimation(FbxScene * pScene)
//{
//
//	FbxArray<FbxString*> AnimStackNameArray;
//	pScene->FillAnimStackNameArray(AnimStackNameArray);
//	int iAnimStackCount = AnimStackNameArray.GetCount();
//
//	for (int i = 0; i < iAnimStackCount; i++)
//	{
//		ParseAnimStack(pScene, AnimStackNameArray.GetAt(i));
//	}
//
//}
//
//void HFbxObj::ParseNodeAnimation(FbxNode* pNode)
//{
//
//	if (pNode == nullptr) return;
//
//	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
//	{
//		return;
//	}
//
//#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
//	auto anim = m_pFbxScene->GetAnimationEvaluator();
//#else
//	auto anim = m_pFbxScene->GetEvaluator();
//#endif
//
//	HStopwatch stopwatch;
//
//	float fCurrentTime = 0.0f;
//
//	auto data = m_hNodeMap.find(pNode);
//	while (fCurrentTime <= m_AnimScene.fEndTime)
//	{
//		FbxTime time;
//		time.SetSecondDouble(fCurrentTime);
//
//		std::string name = pNode->GetName();
//		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, time);
//
//		AddKey(pNode, mat, fCurrentTime);
//
//		/*HAnimTrack track;
//		track.iTick = fCurrentTime * 30 * 160;
//		track.mat = DxConvertMatrix(ConvertMatrixA(mat));
//
//		auto data = m_hNodeMap.find(pNode);
//		data->second->m_AnimList.push_back(track);
//
//		fCurrentTime += m_AnimScene.fDeltaTime;*/
//
//
//		// 1) mat �θ���� ����� ���Ѵ�.
//		// FbxAMatrix self;
//		// 2) self ��� ����( S, R, T )
//		// 3) �ִϸ��̼� Ʈ�� ��� S(����), R(���ʹϾ�), T(����)�� ���ø� �ð� �������� ����
//	}
//	stopwatch.Output(L"AnimNode");
//
//	int iChild = pNode->GetChildCount();
//	for (int iObj = 0; iObj < iChild; iObj++)
//	{
//		FbxNode* pChildNode = pNode->GetChild(iObj);
//		ParseNodeAnimation(pChildNode);
//	}
//
//}
//
//void HFbxObj::ParseAnimStack(FbxScene* pScene, FbxString* pStrAnimStackName)
//{
//
//	FbxAnimStack* anim = pScene->FindMember<FbxAnimStack>(pStrAnimStackName->Buffer());
//	if (anim == nullptr) return;
//
//	FbxTakeInfo* info = pScene->GetTakeInfo(*pStrAnimStackName);
//
//	FbxTime FrameTime;
//	FrameTime.SetTime(0, 0, 0, 1, 0, pScene->GetGlobalSettings().GetTimeMode());
//	float fFrameTime = FrameTime.GetSecondDouble();
//
//	// Tick = 1Sec* FrameSpeed * FramePerTick
//	// 4800 = 1Sec*30* 160;
//	// 0.033 = 1Sec / 30;
//
//	float fStartTime, fEndTime;
//
//	if (info)
//	{
//		// Scene
//		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble();
//		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble();
//
//	}
//	else
//	{
//		FbxTimeSpan timeSpan;
//		pScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timeSpan);
//
//		fStartTime = (float)timeSpan.GetStart().GetSecondDouble();
//		fEndTime = (float)timeSpan.GetStop().GetSecondDouble();
//	}
//
//
//	m_AnimScene.iStartFrame = fStartTime * 30.0f;
//	m_AnimScene.iEndFrame = fEndTime * 30.0f;
//	m_AnimScene.iFrameSpeed = 30;
//	m_AnimScene.iTickPerFrame = 160;
//	m_AnimScene.iDeltaTick = 1;
//	m_AnimScene.fDeltaTime = fFrameTime * 1.0f;
//	m_AnimScene.fStartTime = fStartTime;
//	m_AnimScene.fEndTime = fEndTime;
//
//	// ParseNodeAnimation(pScene->GetRootNode());
//}
//
//
