#pragma once
#include "HNpcObject.h"
#include "HGameUser.h"
#include "HUserInterface.h"
struct HObjAttribute
{
	int iObjState;
	wstring OriginBitmap;
	wstring MaskBitmap;
	wstring PushBitmap;
	wstring SelectBitmap;
	wstring dlsBitmap;
	HPoint  pos;
	RECT rtSrc;
	RECT rtDesk;
	bool bRGBKey;
	DWORD dwRGB;
};

class HScene
{
public:
	vector<HObjAttribute>   m_ObjectAttribute;
	HObjAttribute			m_Objinfo;
	list<BitmapObject*>		m_Object;
	bool					m_bSceneChange;

public:
	bool LoadScript(const TCHAR* filename);
	void ImageCheck(FILE* fp, wstring& name);
	BitmapObject* ObjState(int iState);
	bool Load(const TCHAR* filename);

public:
	// �ʱ�ȭ �� �غ� �۾�
	bool Init();
	// ������ ������ ����۾�
	bool Frame();
	// ������ ������ ��� (��ο�, ������)
	bool Render();
	// �Ҹ� �� ���� ��� ����
	bool Release();
};

