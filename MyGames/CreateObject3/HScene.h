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
	// 초기화 및 준비 작업
	bool Init();
	// 프레임 단위로 계산작업
	bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	bool Render();
	// 소멸 및 삭제 기능 구현
	bool Release();
};

