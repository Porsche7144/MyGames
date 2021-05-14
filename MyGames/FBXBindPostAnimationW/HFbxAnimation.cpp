#include "HFbxObj.h"

bool HFbxObj::ParseMeshSkinningMap(const FbxMesh * pMesh, std::vector<HWeight>& skindata)
{
	// ��Ų�� ����
	int iDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}

	// ������ ����
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

			// skindata�� ����� ��� ���� �� ��.
			// �ε��� ������ �ƴ�.
			int iBoneIndex = data->second;

			// Cluster�� ������ �޴� ������ �ε���
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


		// 1) mat �θ���� ����� ���Ѵ�.
		// FbxAMatrix self;
		// 2) self ��� ����( S, R, T )
		// 3) �ִϸ��̼� Ʈ�� ��� S(����), R(���ʹϾ�), T(����)�� ���ø� �ð� �������� ����
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


