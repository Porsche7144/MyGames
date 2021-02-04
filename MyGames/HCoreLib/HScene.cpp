#include "HScene.h"
#include "HObjectManager.h"

int	HScene::m_iStageCounter = 1;
HScene*	HScene::m_pCurrentScene = nullptr;

HGameUser*	HScene::m_pGamePlayer = nullptr;
bool HScene::GameDataLoad(const TCHAR* pszLoad)
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
		m_rtSpriteList[iCnt].szName = pTemp;
		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, _countof(pTemp),
				&rt.left, &rt.top, &rt.right, &rt.bottom);
			m_rtSpriteList[iCnt].rtArray.push_back(rt);
		}
	}
	fclose(fp_src);
	return true;
}
HObject*  HScene::NewObj(int iType)
{
	HObject* pObject = nullptr;
	switch (iType)
	{
	case HEnum::T_BACKGROUND: {pObject = new HObject; } break;
	case HEnum::T_PLAYER:
	{
		pObject = new HGameUser;
	}break;
	case HEnum::T_NPC: {
		pObject = new HNpcObject;
		HNpcObject* pNpc = dynamic_cast<HNpcObject*>(pObject);
		if (pNpc != nullptr)
		{
			pNpc->FSM();
		}
	} break;
	case HEnum::T_INTERFACE: {pObject = new HUserInterface; } break;
	case HEnum::T_BUTTON: {pObject = new HButton; } break;
	}
	return pObject;
}
void  HScene::GetBitmapLoad(FILE* fp, wstring& ret)
{
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };
	//	nullptr,
	_fgetts(szBuffer, _countof(szBuffer), fp);
	_stscanf_s(szBuffer, _T("%s"), szTemp, _countof(szTemp));
	wstring maskbmp = szTemp;
	if (maskbmp != L"nullptr")
	{
		ret = maskbmp;
	}
}
bool  HScene::LoadScript(const TCHAR* filename)
{
	//std::vector<TObjAttribute>  m_ObjAttribute;
	FILE* fp = nullptr;
	_wfopen_s(&fp, filename, L"rt");
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };

	if (fp != nullptr)
	{
		HObjAttribute objinfo;
		int iMaxCount = 0;
		_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
		_stscanf_s(szBuffer, _T("%d"), &iMaxCount);
		//m_ObjAttribute.resize(iMaxCount);
		for (int iObj = 0; iObj < iMaxCount; iObj++)
		{
			_fgetts(szBuffer, _countof(szBuffer), fp);
			//0 //BACKGROUND
			_fgetts(szBuffer, _countof(szBuffer), fp);
			_stscanf_s(szBuffer, _T("%d"), &objinfo.iObjState);

			GetBitmapLoad(fp, objinfo.OriginBitmap);
			GetBitmapLoad(fp, objinfo.PushBitmap);
			GetBitmapLoad(fp, objinfo.SelectBitmap);
			GetBitmapLoad(fp, objinfo.dlsBitmap);
			GetBitmapLoad(fp, objinfo.MaskBitmap);
			//	0, 0,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%f%f"),
				&objinfo.pos.x,
				&objinfo.pos.y);
			objinfo.rtDesk.left = objinfo.pos.x;
			objinfo.rtDesk.top = objinfo.pos.y;
			//	-1, -1, -1, -1,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"),
				&objinfo.rtSrc.left,
				&objinfo.rtSrc.top,
				&objinfo.rtSrc.right,
				&objinfo.rtSrc.bottom);
			//	-1, -1,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d"),
				&objinfo.rtDesk.right,
				&objinfo.rtDesk.bottom);
			//	0,255, 0, 255
			int mask = 0;
			int r = 0;
			int g = 0;
			int b = 0;
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"),
				&mask,
				&r, &g, &b);
			if (mask == 0)
				objinfo.bRGBKey = false;
			else
				objinfo.bRGBKey = true;
			objinfo.dwRGB = RGB(r, g, b);
			m_ObjAttribute.push_back(objinfo);
		}
	}
	fclose(fp);

	return true;
}
bool  HScene::CreateScene()
{
	m_bSceneChange = false;
	for (HObjAttribute& info : m_ObjAttribute)
	{
		HObject* pObj = NewObj(info.iObjState);

		pObj->Init();
		if (info.bRGBKey == false)
		{
			pObj->Load(info.OriginBitmap.c_str(),
				info.MaskBitmap.c_str());
		}
		else
		{
			pObj->Load(info.OriginBitmap.c_str(),
				nullptr,
				info.dwRGB);
		}
		// state
		pObj->m_StateBitmap.resize(4);
		pObj->m_StateBitmap[DEFAULT] =
			pObj->m_pOriginBmp;
		if (!info.PushBitmap.empty())
		{
			pObj->m_StateBitmap[PUSH] =
				g_BitMgr.Load(info.PushBitmap.c_str());
		}
		if (!info.SelectBitmap.empty())
		{
			pObj->m_StateBitmap[SELECT] =
				g_BitMgr.Load(info.SelectBitmap.c_str());
		}
		if (!info.dlsBitmap.empty())
		{
			pObj->m_StateBitmap[DISABLE] =
				g_BitMgr.Load(info.dlsBitmap.c_str());
		}

		pObj->Set(info.pos, info.rtSrc);

		if (info.iObjState == 101)
		{
			m_pGamePlayer = (HGameUser*)pObj;
			m_ptInitHeroPos = info.pos;
		}
		else
		{
			if (info.iObjState >= 300)
			{
				m_UIObjList.push_back(pObj);
			}
			else
			{
				m_ObjList.push_back(pObj);
			}
		}
	}
	return true;
}
bool  HScene::Load(const TCHAR* filename)
{
	Init();
	if (!LoadScript(filename))
	{
		return false;
	}
	//CreateScene();

	GameDataLoad(L"../../data/script/rtSprite.txt");

	for (int iEffect = 0; iEffect < m_rtSpriteList.size(); iEffect++)
	{
		HEffect* pEffect = new HEffect;
		pEffect->Init();
		pEffect->m_szName = m_rtSpriteList[iEffect].szName;
		pEffect->Load(L"../../data/bitmap/bitmap1.bmp",
			L"../../data/bitmap/bitmap1Mask.bmp");
		RECT rtSrc = m_rtSpriteList[iEffect].rtArray[0];
		HPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(m_rtSpriteList[iEffect].rtArray);
		g_ObjMgr.Add(pEffect);
	}
	return true;
}
bool  HScene::Init()
{
	return true;
}
bool  HScene::Frame()
{
	for (HObject* pObj : m_UIObjList)
	{
		pObj->Frame();
	}
	for (HObject* pObj : m_ObjList)
	{
		if (pObj)	pObj->Frame();
	}
	//effect frame
	for (std::vector<HEffectInfo>::iterator iter = m_EffectList.begin();
		iter != m_EffectList.end();
		)
	{
		iter->Frame();
		if (iter->bDead == true)
		{
			iter = m_EffectList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool  HScene::Release()
{
	for (HObject* pObj : m_ObjList)
	{
		pObj->Release();
		delete pObj;
	}
	for (HObject* pObj : m_UIObjList)
	{
		pObj->Release();
		delete pObj;
	}
	delete m_pGamePlayer;
	m_pGamePlayer = nullptr;
	m_ObjList.clear();
	m_UIObjList.clear();
	m_ObjAttribute.clear();
	return true;
}
bool  HScene::Render()
{
	for (HObject* pObj : m_UIObjList)
	{
		pObj->Render();
	}
	for (HObject* pObj : m_ObjList)
	{
		if (pObj->m_bDead == false)
		{
			pObj->Render();
		}
	}
	for (auto& pInfo : m_EffectList)
	{
		HEffect* pEffect = (HEffect*)g_ObjMgr.GetPtr(pInfo.m_csName);
		pEffect->Set(pInfo.pos,
			pEffect->m_rtList[pInfo.m_iRectIndex]);
		pEffect->Render();
	}
	//int iSave = SetStretchBltMode(g_hOffScreenDC, HALFTONE);
	//int iRet = SetStretchBltMode(g_hOffScreenDC, iSave);
	//m_ButtonColor.DrawColorKey(rtDeskBt, rtSrcBt, RGB(255, 0, 255));
	return true;
}
void  HScene::AddEffect(wstring name, HPoint pos)
{
	HEffect* pEffect = (HEffect*)g_ObjMgr.GetPtr(name);
	if (pEffect == nullptr) return;
	HEffectInfo info;
	info.m_csName = name;
	info.bDead = false;
	info.m_fLIfeTime = 1.0f;
	info.iMaxCount = 12;
	info.m_fStep = 1.0f / pEffect->m_rtList.size();
	info.m_fStep = 0.1f;
	info.pos = { pos.x, pos.y };
	m_EffectList.push_back(info);
}
bool  HScene::Reset()
{
	for (auto obj : m_UIObjList)
	{
		obj->Release();
		delete obj;
	}
	for (auto obj : m_ItemObjList)
	{
		obj->Release();
		delete obj;
	}
	for (auto obj : m_ObjList)
	{
		obj->Release();
		delete obj;
	}

	m_UIObjList.clear();
	m_ItemObjList.clear();
	m_ObjList.clear();
	if (m_pGamePlayer)
	{
		m_pGamePlayer->Reset();
		m_pGamePlayer->SetPos(m_ptInitHeroPos);
	}
	m_EffectList.clear();

	m_iNpcCounter = 0;
	return true;
}
HScene::HScene()
{
	m_iStageCounter = 0;
	m_iSceneID = 0;
	m_bSceneChange = false;
	m_iNextScene = 1;
	m_iNpcCounter = 0;
}
HScene::~HScene()
{

}