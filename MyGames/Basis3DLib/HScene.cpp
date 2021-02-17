#include "HScene.h"
#include "HObjectMgr.h"

int	HScene::m_iStageCounter = 1;
HScene*	HScene::m_pCurrenHScene = nullptr;

HGameUser*	HScene::m_pGamePlayer = nullptr;
LRESULT	 HScene::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	for (auto iter : m_UIObjList)
	{
		HUserInterface* pUI = (HUserInterface*)iter.second;
		int iRet = pUI->MsgProc(hWnd, msg, wParam, lParam);
		if (iRet >= 0) return iRet;
	}
	return -1;
};
HObject*  HScene::NewObj(HObjAttribute& info)
{
	HObject* pObject = nullptr;
	switch (info.iObjType)
	{
		case HEnum::H_MAP: 
		{
			pObject = new HObject; 
		}break;
		case HEnum::H_USER:
		{
			pObject = new HGameUser; 
		}break;
		case HEnum::H_NPC: 
		{
			pObject = new HNpcObject; 
			HNpcObject* pNpc = dynamic_cast<HNpcObject*>(pObject);
			if (pNpc != nullptr)
			{
				pNpc->FSM();
			}
		} break;
		case HEnum::H_INTERFACE: {pObject = new HUserInterface; } break;
		case HEnum::H_DIALOGBOX: 
		{
			pObject = new HDialogBox; 
		} break;
		case HEnum::H_BUTTON: {pObject = new HButton; } break;
		case HEnum::H_EDIT: {pObject = new HEdit; } break;
	}

	return pObject;
}
bool  HScene::CreateScene()
{
	m_bSceneChange = false;
	for (HObjAttribute& info : m_ObjAttribute)
	{
		HObject* pObj = NewObj(info);
		if (pObj == nullptr) continue;

		pObj->m_iObjectType = info.iObjType;
		pObj->m_Attribute = info;
		pObj->m_szName = info.szName;
		pObj->m_szParentName = info.szParentName;
		pObj->Init();				
		
		if (info.bColorKey == false)
		{
			pObj->Load(info.colorbitmap.c_str(),info.maskbitmap.c_str());
		}
		else
		{
			pObj->Load(info.colorbitmap.c_str(),nullptr,info.dwColor);
		}
		//// state
		//pObj->m_StateBitmap.resize(4);
		//pObj->m_StateBitmap[DEFAULT] =	pObj->m_pColorBmp;
		//if (!info.pushbitmap.empty())
		//{
		//	pObj->m_StateBitmap[PUSH] =	g_BitmapMgr.Load(info.pushbitmap.c_str());
		//}
		//if (!info.selectbitmap.empty())
		//{
		//	pObj->m_StateBitmap[SELECT] =g_BitmapMgr.Load(info.selectbitmap.c_str());
		//}
		//if (!info.disbitmap.empty())
		//{
		//	pObj->m_StateBitmap[DISABLE] =	g_BitmapMgr.Load(info.disbitmap.c_str());
		//}
		
		SeHObject(info, pObj);	
		if (info.iObjType == HEnum::H_DIALOGBOX)
		{
			pObj->m_iSortValue += 100;
		}
		if (info.iObjType == HEnum::H_EDIT)
		{
			HEdit* pEdit = (HEdit*)pObj;
			pEdit->m_rtWndCtrl = pObj->m_rtDesk;
			if (pObj->m_pParentObject != nullptr)
			{
				pEdit->m_rtWndCtrl.left = pObj->m_pParentObject->m_rtCollide.left + pObj->m_rtDesk.left;
				pEdit->m_rtWndCtrl.top = pObj->m_pParentObject->m_rtCollide.top + pObj->m_rtDesk.top;
			}
			pEdit->CreateEdit();
		}
	}

	SortZValue(m_UIObjList, m_UIDrawObjList);
	return true;
}
void  HScene::SortZValue(std::map<wstring, HObject*>& srcList, std::vector<std::pair<wstring, HObject*>>& destList)
{
	auto cmp = [](std::pair<wstring, HObject*>  a, std::pair<wstring, HObject*>  b)
	{
		HObject* pA = a.second;
		HObject* pB = b.second;
		return pA->m_iSortValue < pB->m_iSortValue;
	};
	std::copy(srcList.begin(), srcList.end(), std::back_inserter(destList));
	std::sort(destList.begin(), destList.end(), cmp);
}
bool  HScene::SeHObject(HObjAttribute& info, HObject* pObj)
{
	assert(pObj);

	if (pObj && !(info.szParentName == L"nullptr" || info.szParentName == L"NULL"))
	{
		pObj->m_pParentObject = FindParent(info.szParentName);
		if (pObj->m_pParentObject != nullptr)
		{
			pObj->m_pParentObject->m_pChildObjects.push_back(pObj);
		}
	}

	switch (info.iObjType)
	{
	case HEnum::H_USER:
	{
		m_pGamePlayer = (HGameUser*)pObj;
		m_ptInitHeroPos = info.pos;
		return true;
	}break;
	case HEnum::H_NPC:
	{
		pObj->Set(info.pos, info.rtSrc);
		if (pObj->m_pParentObject == nullptr)
		{
			m_CharacterList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case HEnum::H_OBJECT:
	{
		pObj->Set(info.pos, info.rtSrc);
		if (pObj->m_pParentObject == nullptr)
		{
			m_ObjList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case HEnum::H_EDIT:
	{
		pObj->Set(info.rtSrc, info.rtDesk);
		if (pObj->m_pParentObject == nullptr)
		{
			m_UIObjList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case HEnum::H_INTERFACE:
	case HEnum::H_BUTTON:
	case HEnum::H_DIALOGBOX:
	{
		pObj->Set(info.rtSrc, info.rtDesk);
		pObj->Update();
		if (pObj->m_pParentObject == nullptr)
		{
			m_UIObjList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case HEnum::H_MAP:
	{
		pObj->Set(info.rtSrc, info.rtDesk);
		if (pObj->m_pParentObject == nullptr)
		{
			m_MapList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	}
	return false;
}
bool  HScene::Load(const TCHAR* filename)
{
	Init();	
	if (!g_ObjectMgr.LoadObjectFile(filename, m_ObjAttribute))
	{
		return false;
	}
	return true;
}
bool  HScene::Init()
{
	for (auto iter : m_UIObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Init();
	}
	for (auto iter : m_ObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj)	pObj->Init();
	}
	return true;
}
bool  HScene::PreFrame()
{
	return true;
}
bool  HScene::Frame()
{
	for (auto iter : m_MapList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}	
	for (auto iter : m_ObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	for (auto iter : m_NpcList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	for (auto iter : m_UserList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	//effect frame
	for (std::vector<HEffectInfo>::iterator iter = m_EffectList.begin();
		iter != m_EffectList.end();
		)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_EffectList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	for (auto iter : m_UIDrawObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	HButton* pExitBtn = (HButton*)FindUI(L"GAME_EXIT");
	if (pExitBtn &&  pExitBtn->m_bSelect == true)
	{
		HScene::m_pCurrenHScene->m_bGameFinish = true;
	}
	return true;
}
bool  HScene::PostFrame()
{
	return true;
}
bool  HScene::Release()
{
	for (auto iter : m_MapList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ItemObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_UIObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_UserList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_NpcList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	delete m_pGamePlayer;
	m_pGamePlayer = nullptr;
	m_MapList.clear();
	m_ObjList.clear();
	m_ItemObjList.clear();
	m_UIObjList.clear();
	m_ObjAttribute.clear();
	m_UserList.clear();
	m_NpcList.clear();
	m_EffectList.clear();
	return true;
}
bool  HScene::PreRender(ID3D11DeviceContext*	pd3dContext)
{
	return true;
}
bool  HScene::Render(ID3D11DeviceContext*	pd3dContext)
{	
	PreRender(pd3dContext);
	{
		RenderMap(pd3dContext);
		RenderObject(pd3dContext);
		RenderCharacter(pd3dContext);
		RenderEffect(pd3dContext);
		RenderUI(pd3dContext);
	}
	PostRender(pd3dContext);
	return true;
}
bool  HScene::PostRender(ID3D11DeviceContext*	pd3dContext)
{
	if (HScene::m_pCurrenHScene->m_bGameFinish)
	{
		SendMessage(g_hWnd, WM_DESTROY, 0, 0);
		return false;
	}
	return true;
}
bool  HScene::RenderMap(ID3D11DeviceContext*	pd3dContext)
{
	for (auto iter : m_MapList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render(pd3dContext);
		}
	}
	return true;
}
bool  HScene::RenderUI(ID3D11DeviceContext*	pd3dContext)
{
	for (auto iter : m_UIDrawObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render(pd3dContext);
		}
	}
	return true;
}
bool  HScene::RenderEffect(ID3D11DeviceContext*	pd3dContext)
{
	for (auto& pInfo : m_EffectList)
	{
		HEffect* pEffect = (HEffect*)g_ObjectMgr.GetPtr(pInfo.m_csName);
		pEffect->Set(pInfo.p,
			pEffect->m_rtList[pInfo.m_iRectIndex]);
		pEffect->Render(pd3dContext);
	}
	return true;
}
bool  HScene::RenderObject(ID3D11DeviceContext*	pd3dContext)
{
	for (auto iter : m_ObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_bDead == false)
		{
			pObj->Render(pd3dContext);
		}
	}
	return true;
}
bool  HScene::RenderCharacter(ID3D11DeviceContext*	pd3dContext)
{
	for (auto iter : m_NpcList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render(pd3dContext);
		}
	}
	for (auto iter : m_UserList)
	{
		HObject* pObj = (HObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render(pd3dContext);
		}
	}
	return true;
}
void  HScene::AddEffect(wstring name, HPoint pos, HPoint dir)
{
	HEffect* pEffect = (HEffect*)g_ObjectMgr.GetPtr(name);
	if (pEffect == nullptr) return;
	HEffectInfo info;
	info.m_csName = name;
	info.m_bDead = false;
	info.m_fLifeTime = 1.0f;
	info.iMaxCount = (int)pEffect->m_rtList.size();
	info.m_fStep = 1.0f / pEffect->m_rtList.size();
	info.m_fStep = 0.1f;
	info.d = { dir.x, dir.y };
	info.p = { pos.x, pos.y };
	m_EffectList.push_back(info);
}
void  HScene::AddNpc(wstring name, HPoint pos, HPoint dir, int index)
{
};
bool  HScene::Reset()
{
	for (auto iter : m_UIObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ItemObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ObjList)
	{
		HObject* pObj = (HObject*)iter.second;
		pObj->Release();
		delete pObj;
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
HObject*	HScene::FindParent(std::wstring szName)
{
	HObject* pParentObj = FindMap(szName);
	if (pParentObj == nullptr)
	{
		pParentObj = FindUI(szName);
		if (pParentObj == nullptr)
		{
			pParentObj = FindItem(szName);
			if (pParentObj == nullptr)
			{
				pParentObj = FindObject(szName);
				if (pParentObj == nullptr)
				{
					return nullptr;
				}
			}
		}
	}
	return pParentObj;
}
HObject*	HScene::FindMap(std::wstring szName)
{
	auto iter = m_MapList.find(szName);
	if (iter == m_MapList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
HObject*	HScene::FindUI(std::wstring szName)
{
	auto iter = m_UIObjList.find(szName);
	if (iter == m_UIObjList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
HObject*	HScene::FindItem(std::wstring szName)
{
	auto iter = m_ItemObjList.find(szName);
	if (iter == m_ItemObjList.end())
	{
		return nullptr;
	}
	return (*iter).second;
};
HObject*	HScene::FindObject(std::wstring szName)
{
	auto iter = m_ObjList.find(szName);
	if (iter == m_ObjList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
HObject*	HScene::FindNpc(int iIndex)
{
	auto iter = m_NpcList.find(iIndex);
	if (iter == m_NpcList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
bool	HScene::DelNpc(int iIndex)
{
	auto iter = m_NpcList.find(iIndex);
	if (iter != m_NpcList.end())
	{
		g_ObjectMgr.DeleteExecute(iter->second);
		delete iter->second;
		m_NpcList.erase(iter);		
		return true;
	}
	return false;
}
HScene::HScene()
{
	m_iStageCounter = 0;
	m_iSceneID		= 0;
	m_bSceneChange	= false;
	m_iNexHScene	= HScene_LOADING;
	m_iNpcCounter	= 0;
}
HScene::~HScene()
{

}