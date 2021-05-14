#include "HFbxObj.h"

bool HFbxObj::ParseMeshSkinningMap(const FbxMesh * pMesh, std::vector<HWeight>& skindata)
{
	// 스킨의 갯수
	int iDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}

	// 제어점 갯수
	int iVertexCount = pMesh->GetControlPointsCount();
	skindata.resize(iVertexCount);

	for (int iDeformerIndex = 0; iDeformerIndex < iDeformerCount; iDeformerIndex++)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(iDeformerIndex, FbxDeformer::eSkin));
		int iClusterCount = pSkin->GetClusterCount();

		for (int iClusterIndex = 0; iClusterIndex < iClusterCount; iClusterIndex++)
		{
			auto pCluster = pSkin->GetCluster(iClusterIndex);
			int iClusterSize = pCluster->GetControlPointIndicesCount();

			auto data = m_pNodeMap.find(pCluster->GetLink());

			// skindata에 저장된 노드 순서 일 뿐.
			// 인덱스 순서가 아님.
			int iBoneIndex = data->second;

			// Cluster의 영향을 받는 정점들 인덱스
			int* pIndeces = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();

			for (int i = 0; i < iClusterSize; i++)
			{
				skindata[pIndeces[i]].InsertWeight(iBoneIndex, pWeights[i]);
			}

		}

	}

	return true;
}

void HFbxObj::ParseAnimation(FbxScene * pScene)
{

	FbxArray<FbxString*> AnimStackNameArray;
	pScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();

	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(pScene, AnimStackNameArray.GetAt(i));
	}

}

void HFbxObj::ParseNodeAnimation(FbxNode* pNode)
{

	if (pNode == nullptr) return;

	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}

	auto anim = m_pFbxScene->GetAnimationEvaluator();

	float fCurrentTime = 0.0f;

	while (fCurrentTime <= m_AnimScene.fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);

		std::string name = pNode->GetName();
		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, time);

		HAnimTrack track;
		track.iTick = fCurrentTime * 30 * 160;
		track.mat = DxConvertMatrix(ConvertMatrixA(mat));

		auto data = m_hNodeMap.find(pNode);
		data->second->m_AnimList.push_back(track);

		fCurrentTime += m_AnimScene.fDeltaTime;


		// 1) mat 부모행렬 역행렬 곱한다.
		// FbxAMatrix self;
		// 2) self 행렬 분해( S, R, T )
		// 3) 애니메이션 트랙 행렬 S(벡터), R(쿼터니언), T(벡터)를 샘플링 시간 간격으로 저장
	}


	int iChild = pNode->GetChildCount();
	for (int iObj = 0; iObj < iChild; iObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(iObj);
		ParseNodeAnimation(pChildNode);
	}

}

void HFbxObj::ParseAnimStack(FbxScene* pScene, FbxString* pStrAnimStackName)
{

	FbxAnimStack* anim = pScene->FindMember<FbxAnimStack>(pStrAnimStackName->Buffer());
	if (anim == nullptr) return;

	FbxTakeInfo* info = pScene->GetTakeInfo(*pStrAnimStackName);

	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, pScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();

	// Tick = 1Sec* FrameSpeed * FramePerTick
	// 4800 = 1Sec*30* 160;
	// 0.033 = 1Sec / 30;

	float fStartTime, fEndTime;

	if (info)
	{
		// Scene
		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble();
		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble();

	}
	else
	{
		FbxTimeSpan timeSpan;
		pScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timeSpan);

		fStartTime = (float)timeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)timeSpan.GetStop().GetSecondDouble();
	}


	m_AnimScene.iStartFrame = fStartTime * 30.0f;
	m_AnimScene.iEndFrame = fEndTime * 30.0f;
	m_AnimScene.iFrameSpeed = 30;
	m_AnimScene.iTickPerFrame = 160;
	m_AnimScene.iDeltaTick = 1;
	m_AnimScene.fDeltaTime = fFrameTime * 1.0f;
	m_AnimScene.fStartTime = fStartTime;
	m_AnimScene.fEndTime = fEndTime;

	ParseNodeAnimation(pScene->GetRootNode());
}


