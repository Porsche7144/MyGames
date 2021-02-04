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
	// �ʱ�ȭ �� �غ� �۾�
	virtual bool Init();
	// ������ ������ ����۾�
	virtual bool Frame();
	// ������ ������ ��� (��ο�, ������)
	virtual bool Render();
	// �Ҹ� �� ���� ��� ����
	virtual bool Release();

public:
	HScene()
	{
		m_iSceneID = 0;
		m_bSceneChange = false;
	}
};

