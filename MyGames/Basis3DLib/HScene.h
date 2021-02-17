#pragma once
#include "HUserInterface.h"
#include "HGameUser.h"
#include "HNpcObject.h"
#include "HEffect.h"

struct HEnum
{
	enum  HGameType 
	{
		H_MAP = 0,
		H_USER = 100,
		H_NPC = 200,
		H_INTERFACE = 300,
		H_BUTTON,
		H_EDIT,
		H_DIALOGBOX,
		H_EFFECT = 400,
		H_OBJECT = 500,
	};
};


class HScene
{
public:
	enum HSceneID
	{
		HScene_LOADING = 0,
		HScene_LOBBY = 1,
		HScene_ZONE = 2,
	};
public:
	static HScene*				m_pCurrenHScene;
	std::vector<HObjAttribute>  m_ObjAttribute;
	static HGameUser*			m_pGamePlayer;	
	std::vector<HSpriteInfo>	m_rtSpriteList;

	std::map<wstring, HObject*>				m_UIObjList;
	std::map<wstring, HObject*>				m_ItemObjList;
	std::map<wstring, HObject*>				m_ObjList;
	std::map<wstring, HObject*>				m_CharacterList;
	std::map<wstring, HObject*>				m_MapList;
	std::map<wstring, HObject*>::iterator	m_iter;

	std::vector<std::pair<wstring, HObject*>>	m_UIDrawObjList;
public:
	virtual HObject*	FindUI(std::wstring szName);
	virtual HObject*	FindItem(std::wstring szName);
	virtual HObject*	FindObject(std::wstring szName);
	virtual HObject*	FindMap(std::wstring szName);
	virtual HObject*	FindNpc(int iIndex);
	virtual HObject*	FindParent(std::wstring szName);
	virtual bool		DelNpc(int iIndex);
public:
	std::map<int, HGameUser*>			m_UserList;
	std::map<int, HObject*>				m_NpcList;
public:
	static int	m_iStageCounter;
	HPoint		m_ptInitHeroPos;
	int			m_iNexHScene;
	int			m_iNpcCounter;
	int			m_iMaxCounter;
	bool		m_bGameFinish;
	int			m_iSceneID;
	bool		m_bSceneChange;
public:
	std::vector<HEffectInfo>	m_EffectList;
	virtual void  AddEffect(wstring name, HPoint pos, HPoint dir);
	virtual void  AddNpc(wstring name, HPoint pos, HPoint dir, int index);
	virtual bool  Reset();
	virtual void  SortZValue(	std::map<wstring, HObject*>& srcList,
								std::vector<std::pair<wstring, HObject*>>& destList);
public:
	virtual LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual bool  Load(const TCHAR* filename);
	virtual bool  Init();
	virtual bool  PreFrame();
	virtual bool  Frame();
	virtual bool  PostFrame();
	virtual bool  PreRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool  Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool  PostRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderMap(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderObject(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderCharacter(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderEffect(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderUI(ID3D11DeviceContext*	pd3dContext);
	virtual bool  Release();
public:
	virtual HObject*  NewObj(HObjAttribute& info);
	virtual bool  CreateScene();
	virtual bool  SeHObject(HObjAttribute& iType, HObject* pObj);
public:
	HScene();
	virtual ~HScene();

};

