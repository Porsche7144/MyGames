#include "Sample.h"

bool Sample::GameDataLoad(const TCHAR* pszLoad)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };

	int iNumSprite = 0;
	FILE* fp_src;
	_wfopen_s(&fp_src, pszLoad, _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iNumSprite);
	m_rtSpriteList.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d"), pTemp, _countof(pTemp), &iNumFrame);
		//m_rtSpriteList[iCnt].resize(iNumFrame);

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, _countof(pTemp),
				&rt.left, &rt.top, &rt.right, &rt.bottom);
			m_rtSpriteList[iCnt].push_back(rt);
		}
	}
	fclose(fp_src);
	return true;
}


// 초기화 및 준비 작업
bool Sample::Init()
{
	GameDataLoad(L"rtSpriteInfo.txt");

	for (int iEffect = 0; iEffect < m_rtSpriteList.size(); iEffect++)
	{
		HEffect* pEffect = new HEffect;
		pEffect->Init();
		pEffect->Load(L"../../data/bitmap/bitmap1.bmp",
			L"../../data/bitmap/bitmap1Mask.bmp");
		RECT rtSrc = m_rtSpriteList[iEffect][0];
		HPoint pos = { 0,0 };
		pEffect->Set(pos, rtSrc);
		pEffect->SetSprite(m_rtSpriteList[iEffect]);
		m_EffectList.push_back(pEffect);
		
	}

	return true;
}
// 프레임 단위로 계산작업
bool Sample::Frame()
{
	if (g_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
	{
		HEffectInfo info;
		info.bDead = false;
		info.id = rand() % 3;
		info.iMaxCount = m_EffectList[info.id]->m_rtList.size();
		info.m_fStep = 1.0f / m_EffectList[info.id]->m_rtList.size();
		info.pos = { g_Input.GetPos().x, g_Input.GetPos().y };

		m_EffectInfoList.push_back(info);
	}

	for (vector<HEffectInfo>::iterator iter = m_EffectInfoList.begin(); 
									   iter != m_EffectInfoList.end();)
	{
		iter->Frame();
		if (iter->bDead == true)
		{
			iter = m_EffectInfoList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	return true;
}
// 프레임 단위로 출력 (드로우, 렌더링)
bool Sample::Render()
{
	for (auto& pInfo : m_EffectInfoList)
	{
		HPoint p = { pInfo.pos.x, pInfo.pos.y };
		m_EffectList[pInfo.id]->Set(p, m_EffectList[pInfo.id]->m_rtList[pInfo.m_iRectIndex]);
		
		m_EffectList[pInfo.id]->Render();
	}
	return true;
}
// 소멸 및 삭제 기능 구현
bool Sample::Release()
{
	for (auto pEffect : m_EffectList)
	{
		pEffect->Release();
	}
	return true;
}

GAMERUN;
