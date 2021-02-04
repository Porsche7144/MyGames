#pragma once
#include "HUserInterface.h"
#include "HGameUser.h"
#include "HNpcObject.h"
#include "HEffect.h"

struct HEnum
{
	enum  HGameType
	{
		T_BACKGROUND = 0,
		T_PLAYER = 100,
		T_NPC = 200,
		T_INTERFACE = 300,
		T_BUTTON,
		T_EFFECT = 400,
	};
};

struct HSpriteInfo
{
	RECT_ARRAY rtArray;
	wstring    szName;
};
class HScene
{
public:
	static HScene*				m_pCurrentScene;
	std::vector<HObjAttribute>  m_ObjAttribute;
	std::vector<HObject*>		m_UIObjList;
	std::vector<HObject*>		m_ItemObjList;
	std::vector<HObject*>		m_ObjList;
	static HGameUser*			m_pGamePlayer;
	std::vector<HSpriteInfo>	m_rtSpriteList;
public:
	std::list<HGameUser>		m_UserList;
public:
	static int	m_iStageCounter;
	HPoint		m_ptInitHeroPos;
	int			m_iNextScene;
	int			m_iNpcCounter;
	int			m_iMaxCounter;
	bool		m_bGameFinish;
	int			m_iSceneID;
	bool		m_bSceneChange;
public:
	std::vector<HEffectInfo>	m_EffectList;
	void  AddEffect(wstring name, HPoint pos);
	bool  GameDataLoad(const TCHAR* pszLoad);
	virtual bool  Reset();
public:
	void  GetBitmapLoad(FILE* fp, wstring& ret);
	bool  Load(const TCHAR* filename);
	virtual bool  Init();
	virtual bool  Frame();
	virtual bool  Render();
	virtual bool  Release();
public:
	virtual HObject*  NewObj(int iType);
	virtual bool  LoadScript(const TCHAR* filename);
	virtual bool  CreateScene();
public:
	HScene();
	virtual ~HScene();

};

