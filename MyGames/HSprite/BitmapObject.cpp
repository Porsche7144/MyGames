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

void BitmapObject::SetPos(HPoint p)
{
	m_ptPos = p;
	Set(p, m_rtSrc, m_fSpeed);
}

void BitmapObject::Set(RECT rtSrc, RECT rtDesk, float fSpeed)
{
	m_fSpeed = fSpeed;
	if (rtSrc.left >= 0) m_rtSrc.left = rtSrc.left;
	if (rtSrc.top >= 0) m_rtSrc.top = rtSrc.top;
	if (rtSrc.right >= 0) m_rtSrc.right = rtSrc.right;
	if (rtSrc.bottom >= 0) m_rtSrc.bottom = rtSrc.bottom;
	if (rtDesk.left >= 0) m_rtDesk.left = rtDesk.left;
	if (rtDesk.top >= 0) m_rtDesk.top = rtDesk.top;
	if (rtDesk.right >= 0) m_rtDesk.right = rtDesk.right;
	if (rtDesk.bottom >= 0) m_rtDesk.bottom = rtDesk.bottom;
	m_ptPos.x = m_rtDesk.right;
	m_ptPos.y = m_rtDesk.top;
}

void BitmapObject::Set(HPoint pos, RECT rtSrc, float fSpeed)
{
	m_ptPos = pos;
	m_fSpeed = fSpeed;
	if (rtSrc.left >= 0) m_rtSrc.left = rtSrc.left;
	if (rtSrc.top >= 0) m_rtSrc.top = rtSrc.top;
	if (rtSrc.right >= 0) m_rtSrc.right = rtSrc.right;
	if (rtSrc.bottom >= 0) m_rtSrc.bottom = rtSrc.bottom;
	m_rtDesk.left = m_ptPos.x - (m_rtSrc.right / 2);
	m_rtDesk.top = m_ptPos.y - (m_rtSrc.bottom / 2);
	m_rtDesk.right = m_rtSrc.right;
	m_rtDesk.bottom = m_rtSrc.bottom;
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