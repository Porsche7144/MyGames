#pragma once
#include "HNpcObject.h"
#include "HGameUser.h"
#include "HUserInterface.h"

class HScene
{
public:
	vector<HObjAttribute>   m_ObjectAttribute;
	HObjAttribute			m_Objinfo;
	vector<BitmapObject*>	m_Object;
	vector<BitmapObject*>	m_ItemObject;
	vector<BitmapObject*>	m_UIObject;
	bool					m_bSceneChange;
	int						m_iSceneID;
	HGameUser*				m_pGamePlayer;

public:
	bool LoadScript(const TCHAR* filename);
	void ImageCheck(FILE* fp, wstring& name);
	BitmapObject* ObjState(int iState);
	bool Load(const TCHAR* filename);

public:
	// 초기화 및 준비 작업
	virtual bool Init();
	// 프레임 단위로 계산작업
	virtual bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	virtual bool Render();
	// 소멸 및 삭제 기능 구현
	virtual bool Release();

public:
	HScene()
	{
		m_iSceneID = 0;
		m_bSceneChange = false;
	}
};

