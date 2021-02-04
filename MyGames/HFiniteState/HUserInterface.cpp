#include "HUserInterface.h"



bool HButton::Frame()
{
	m_iStateValue = 0;
	if (HCollision::RectPt(m_rtDesk, g_Input.GetPos()))
	{
		m_iStateValue = 1;
		if (g_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
		{
			m_iStateValue = 2;
		}
	}

	return true;
}

bool HButton::Render()
{
	if (m_bBmpKey == true)
	{
		HBitmap* pCurrent = m_pOriginBmp;
		(m_StateBitmap[m_iStateValue] != nullptr) ? 
			pCurrent = m_StateBitmap[m_iStateValue] :
			pCurrent = nullptr;

		if (pCurrent != nullptr)
		{
			pCurrent->DrawTransParent(m_rtDesk, m_rtSrc, m_dwBmpKey);
		}
		return true;
	}

	if (m_pOriginBmp && m_pOriginBmp->m_BitmapInfo.bmBitsPixel == 32)
	{
		switch (m_iStateValue)
		{
		case 0: {m_pOriginBmp->DrawAlphaBlend(m_rtDesk, m_rtSrc); }
				break;
		case 1: {m_StateBitmap[1]->DrawAlphaBlend(m_rtDesk, m_rtSrc); }
				break;
		case 2: {m_StateBitmap[2]->DrawAlphaBlend(m_rtDesk, m_rtSrc); }
				break;
		case 3: {m_StateBitmap[3]->DrawAlphaBlend(m_rtDesk, m_rtSrc); }
				break;
		}
	}

	if (m_pMaskBmp != nullptr)
	{
		m_pMaskBmp->Draw(m_rtDesk, m_rtSrc, SRCAND, 0);
		m_pOriginBmp->Draw(m_rtDesk, m_rtSrc, SRCINVERT, 0);
		m_pMaskBmp->Draw(m_rtDesk, m_rtSrc, SRCINVERT, 0);
	}
	else
	{
		switch (m_iStateValue)
		{
		case 0: {m_pOriginBmp->Render(); }
			  break;
		case 1: {m_StateBitmap[1]->Render(); }
			  break;
		case 2: {m_StateBitmap[2]->Render(); }
			  break;
		case 3: {m_StateBitmap[3]->Render(); }
			  break;
		}
	}
	return true;
}

HUserInterface::~HUserInterface()
{

}