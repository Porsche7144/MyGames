#pragma once
#include "TEffect.h"
struct cmp {
	bool operator()(int i, int j)
	{
		if (i > j) return true;
		else return false;
	}
};
class TObjectComposite : public TObject
{
public:
	std::deque<TObject*> m_Components;
public:
	TObject* Clone();
};
class TObjectManager : public TSingleton<TObjectManager>
{
public:
	friend  class TSingleton<TObjectManager>;
private:
	std::vector<TObjAttribute>  m_ObjAttribute;
	std::vector<TSpriteInfo>	m_rtSpriteList;
private:
	std::map<wstring, TObject*>				m_List;
	std::map<wstring, TObject*>::iterator	m_iter;
	std::wstring				m_szDefaultPath;
public:
	TObject*	Add(TObject* pAddObject);
	bool		Load(const TCHAR* filename);
	TObject*	GetPtr(wstring szName);
	TObject*	Clone(wstring szName);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	bool  LoadObjectFile(T_STR szFileName, std::vector<TObjAttribute>&);
	bool  LoadEffectFile(T_STR szFileName, std::vector<TSpriteInfo>&);
	void  CreateEffect(std::vector<TSpriteInfo>&	list);
	void  CreateObject(std::vector<TObjAttribute>&	list);
	void  GetBitmapLoad(FILE* fp, wstring& ret);
private:
	TObjectManager();
public:
	~TObjectManager();
};
#define g_ObjectMgr TObjectManager::GetInstance()

