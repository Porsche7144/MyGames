#pragma once
#include "HEffect.h"
#include <functional>
struct cmp {
	bool operator()(int i, int j)
	{
		if (i > j) return true;
		else return false;
	}
};

class HObjectComposite : public HObject
{
public:
	std::deque<HObject*> m_Components;
public:
	HObject* Clone();
};

using CollisionFunction = std::function<void(HObject*, DWORD)>;
using SelectFunction = std::function<void(POINT, DWORD)>;

class HObjectManager : public HSingleton<HObjectManager>
{
public:
	friend  class HSingleton<HObjectManager>;
private:
	std::vector<HObjAttribute>  m_ObjAttribute;
	std::vector<HSpriteInfo>	m_rtSpriteList;
private:
	std::map<wstring, HObject*>				m_List;
	std::map<wstring, HObject*>::iterator	m_iter;
	std::wstring				m_szDefaultPath;
private:
	int		m_iExecuteSelectID = 0;
	int		m_iExecuteCollisionID = 0;
	std::map<int ,HObject*>	      m_CollisionObjectList;
	std::map<int, HObject*>	      m_SelecHObjectList;

	typedef std::map<int, SelectFunction>::iterator SelectFuncIterator;
	std::map<int, SelectFunction>    m_fnSelectExecute;

	typedef std::map<int, CollisionFunction>::iterator CollisionFuncIterator;
	std::map<int, CollisionFunction>    m_fnCollisionExecute;
public:
	void		AddSelectExecute(HObject* ownder, SelectFunction func);
	void		AddCollisionExecute(HObject* ownder, CollisionFunction func);
	void		DeleteExecute(HObject* owner);
public:
	HObject*	Add(HObject* pAddObject);
	bool		Load(const TCHAR* filename);
	HObject*	GetPtr(wstring szName);
	HObject*	Clone(wstring szName);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	bool  LoadObjectFile(T_STR szFileName, std::vector<HObjAttribute>&);
	bool  LoadEffectFile(T_STR szFileName, std::vector<HSpriteInfo>&);
	void  CreateEffect(std::vector<HSpriteInfo>&	list);
	void  CreateObject(std::vector<HObjAttribute>&	list);
	void  GetBitmapLoad(FILE* fp, wstring& ret);
private:
	HObjectManager();
public:
	~HObjectManager();
};
#define g_ObjectMgr HObjectManager::GetInstance()

