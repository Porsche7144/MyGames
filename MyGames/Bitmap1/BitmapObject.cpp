#include "BitmapObject.h"

bool BitmapObject::Init()
{
	m_pOriginBmp = nullptr;
	m_pMaskBmp = nullptr;
	return true;
}

void BitmapObject::SetPosition(HPoint p)
{
	m_ptPos = p;
	m_rtDesk.left = p.x;
	m_rtDesk.top = p.y;
}

void BitmapObject::Set(RECT rtSrc, RECT rtDesk, float fSpeed)
{
	m_fSpeed = fSpeed;
	m_rtSrc = rtSrc;
	m_rtDesk = rtDesk;
	m_ptPos.x = m_rtDesk.right;
	m_ptPos.y = m_rtDesk.top;
}

bool BitmapObject::Load(const TCHAR* Original, const TCHAR* Mask)
{
	if (Original != nullptr)
	{
		m_pOriginBmp = g_BitMgr.Load(Original);
		if (m_pOriginBmp != nullptr)
		{
			Set({ 0,0, m_pOriginBmp->m_BitmapInfo.bmWidth, m_pOriginBmp->m_BitmapInfo.bmHeight },
				{ 0,0, m_pOriginBmp->m_BitmapInfo.bmWidth, m_pOriginBmp->m_BitmapInfo.bmHeight });
		}
	}
	if (Mask != nullptr)
	{
		m_pMaskBmp = g_BitMgr.Load(Mask);
	}
	return true;
}

bool BitmapObject::Load(const TCHAR* Original, DWORD dwMask)
{
	if (Original != nullptr)
	{
		m_pOriginBmp = g_BitMgr.Load(Original);
		if (m_pOriginBmp != nullptr)
		{
			Set({ 0,0, m_pOriginBmp->m_BitmapInfo.bmWidth, m_pOriginBmp->m_BitmapInfo.bmHeight },
				{ 0,0, m_pOriginBmp->m_BitmapInfo.bmWidth, m_pOriginBmp->m_BitmapInfo.bmHeight });
		}
	}
	m_bBmpKey = true;
	m_dwBmpKey = dwMask;
	return true;
}

bool BitmapObject::Render()
{
	if (m_bBmpKey == true)
	{
		DrawKey();
	}
	if (m_pMaskBmp != nullptr)
	{
		m_pMaskBmp->Draw(m_rtDesk, m_rtSrc, SRCAND, 0);
		m_pOriginBmp->Draw(m_rtDesk, m_rtSrc, SRCINVERT, 0);
		m_pMaskBmp->Draw(m_rtDesk, m_rtSrc, SRCINVERT, 0);
	}
	else
	{
		m_pOriginBmp->Render();
	}

	return true;
}

bool BitmapObject::Frame()
{
	return true;
}

bool BitmapObject::DrawKey()
{
	m_pOriginBmp->DrawTransParent(m_rtDesk, m_rtSrc, m_dwBmpKey);
	return true;
}

bool BitmapObject::Release()
{
	return true;
}

BitmapObject::BitmapObject()
{
	m_fSpeed = 200.0f;
	m_bBmpKey = false;
	m_dwBmpKey = RGB(255, 255, 255);
}

BitmapObject::~BitmapObject()
{

}