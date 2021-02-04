#include "HScene.h"

BitmapObject* HScene::ObjState(int iState)
{
	BitmapObject* pObject = nullptr;
	switch (iState)
	{
	case 0: // BackGround
	{pObject = new HButton; } break;
	case 1: // UI
	{pObject = new HUserInterface; } break;
	case 2: // User
	{pObject = new HGameUser; } break;
	case 3: // NPC
	{pObject = new HNpcObject; } break;
	}
	return pObject;
}

void HScene::ImageCheck(FILE* fp, wstring& name)
{
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };

	//wstring MaskBitmap;
	_fgetts(szBuffer, _countof(szBuffer), fp);
	_stscanf_s(szBuffer, _T("%s"), szTemp, _countof(szTemp));
	wstring MaskBmp = szTemp;
	if (MaskBmp != L"nullptr")
	{
		name = MaskBmp;
	}
}

bool HScene::LoadScript(const TCHAR* filename)
{
	FILE* fp = nullptr;
	_wfopen_s(&fp, filename, L"rt");
	TCHAR szBuffer[256] = { 0, };

	if (fp != nullptr)
	{
		int iCnt = 0;
		_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
		_stscanf_s(szBuffer, _T("%d"), &iCnt);

		for (int i = 0; i < iCnt; i++)
		{
			// \n
			_fgetts(szBuffer, _countof(szBuffer), fp);
			//int iObjState;
			_fgetts(szBuffer, _countof(szBuffer), fp);
			_stscanf_s(szBuffer, _T("%d"), &m_Objinfo.iObjState);

			ImageCheck(fp, m_Objinfo.OriginBitmap);
			ImageCheck(fp, m_Objinfo.PushBitmap);
			ImageCheck(fp, m_Objinfo.SelectBitmap);
			ImageCheck(fp, m_Objinfo.dlsBitmap);
			ImageCheck(fp, m_Objinfo.MaskBitmap);
			//HPoint  pos;
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%f%f"), &m_Objinfo.pos.x, &m_Objinfo.pos.y);

			m_Objinfo.rtDesk.left = m_Objinfo.pos.x;
			m_Objinfo.rtDesk.top = m_Objinfo.pos.y;

			//RECT rtSrc;  -1 -1 -1 -1
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"),
				&m_Objinfo.rtSrc.left, &m_Objinfo.rtSrc.top,
				&m_Objinfo.rtSrc.right, &m_Objinfo.rtSrc.bottom);

			//RECT rtDesk; -1 -1
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d"),
				&m_Objinfo.rtDesk.right, &m_Objinfo.rtDesk.bottom);

			//DWORD dwRGB;
			int mask = 0;
			int r = 0;
			int g = 0;
			int b = 0;
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"),
				&mask, &r, &g, &b);
			if (mask == 0)
			{
				m_Objinfo.bRGBKey = false;
			}
			else
			{
				m_Objinfo.bRGBKey = true;
				m_Objinfo.dwRGB = RGB(r, g, b);
			}
			// Objinfo.dwRGB = RGB(r, g, b);
			m_ObjectAttribute.push_back(m_Objinfo);
		}
	}
	fclose(fp);

	return true;
}

bool HScene::Load(const TCHAR* filename)
{
	if (!LoadScript(filename))
	{
		return false;
	}

	for (HObjAttribute& Info : m_ObjectAttribute)
	{
		BitmapObject* pObj = ObjState(Info.iObjState);
		pObj->Init();
		if (Info.bRGBKey == false)
		{
			pObj->Load(Info.OriginBitmap.c_str(), Info.MaskBitmap.c_str());
		}
		else
		{
			pObj->Load(Info.OriginBitmap.c_str(), Info.dwRGB);
		}
		pObj->SetPosition(Info.pos);
		pObj->Set(Info.rtSrc, Info.rtDesk);
		m_Object.push_back(pObj);

		if (!Info.PushBitmap.empty())
		{
			pObj->m_StateBitmap.push_back(g_BitMgr.Load(Info.PushBitmap.c_str()));
		}
		if (!Info.SelectBitmap.empty())
		{
			pObj->m_StateBitmap.push_back(g_BitMgr.Load(Info.SelectBitmap.c_str()));
		}
		if (!Info.dlsBitmap.empty())
		{
			pObj->m_StateBitmap.push_back(g_BitMgr.Load(Info.dlsBitmap.c_str()));
		}
		pObj->SetPosition(Info.pos);
		pObj->Set(Info.rtSrc, Info.rtDesk);
		m_Object.push_back(pObj);
	}
	return true;
}

bool HScene::Init()
{
	return true;
}

bool HScene::Frame()
{
	for (BitmapObject* pBitObj : m_Object)
	{
		pBitObj->Frame();
	}
	return true;
}
// 프레임 단위로 출력 (드로우, 렌더링)
bool HScene::Render()
{
	for (BitmapObject* pBitObj : m_Object)
	{
		pBitObj->Render();
	}
	return true;
}
// 소멸 및 삭제 기능 구현
bool HScene::Release()
{
	for (BitmapObject* pBitObj : m_Object)
	{
		pBitObj->Render();
		delete pBitObj;
	}
	m_Object.clear();
	m_ObjectAttribute.clear();
	return true;
}