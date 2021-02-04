#include "HInput.h"

HInputKey	g_ActionInput;
POINT		g_MousePos;

DWORD HInput::GetKey(DWORD dKey)
{
	return m_dwKeyState[dKey];
}

// ÃÊ±âÈ­
bool HInput::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	return true;
}

bool HInput::Frame()
{
	// ¸¶¿ì½º ÁÂÇ¥
	//GetCursorPos(&g_MousePos);
	//ScreenToClient(g_hwnd, &g_MousePos);
	GetCursorPos(&m_iStateValue); // È­¸éÁÂÇ¥°è
	ScreenToClient(g_hwnd, &m_iStateValue);

	for (int iKey = 0; iKey < 256; iKey++)
	{
		SHORT sKey = GetAsyncKeyState(iKey);
		// 00000000 00000000
		// 1000 0000
		//     &
		// 1000 0000
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[iKey] == KEY_FREE)
			{
				m_dwKeyState[iKey] = KEY_PUSH;
			}
			else
			{
				m_dwKeyState[iKey] = KEY_HOLD;
			}
		}
		else
		{
			if (m_dwKeyState[iKey] == KEY_PUSH ||
				m_dwKeyState[iKey] == KEY_HOLD)
			{
				m_dwKeyState[iKey] = KEY_UP;
			}
			else
			{
				m_dwKeyState[iKey] = KEY_FREE;
			}
		}
	}

	g_ActionInput.bWKey = false;
	g_ActionInput.bAKey = false;
	g_ActionInput.bDKey = false;
	g_ActionInput.bSKey = false;
	g_ActionInput.bLeftClick = false;

	if (m_dwKeyState[VK_UP] || m_dwKeyState['W'])
	{
		g_ActionInput.bWKey = true; // up, w
	}
	if (m_dwKeyState[VK_LEFT] || m_dwKeyState['A'])
	{
		g_ActionInput.bAKey = true; // up, wwd
	}
	if (m_dwKeyState[VK_RIGHT] || m_dwKeyState['D'])
	{
		g_ActionInput.bDKey = true; // up, w
	}
	if (m_dwKeyState[VK_DOWN] || m_dwKeyState['S'])
	{
		g_ActionInput.bSKey = true; // up, ws
	}
	if (m_dwKeyState[VK_RETURN] || m_dwKeyState[VK_MBUTTON])
	{
		g_ActionInput.bMiddleClick = true; // up, w
	}
	if (m_dwKeyState[VK_SPACE] || m_dwKeyState[VK_RBUTTON])
	{
		g_ActionInput.bSpaceKey = true; // up, w
	}
	if (m_dwKeyState[VK_LBUTTON])
	{
		g_ActionInput.bLeftClick = true; // up, w
	}

	SHORT sKey = GetKeyState(VK_NUMLOCK);
	g_ActionInput.bNumLock = false;
	g_ActionInput.bCapsLock = false;
	g_ActionInput.bHangul = false;
	if (sKey & 0x01)
	{
		g_ActionInput.bNumLock = true;
	}
	sKey = GetKeyState(VK_CAPITAL);
	if (sKey & 0x01)
	{
		g_ActionInput.bCapsLock = true;
	}
	sKey = GetKeyState(VK_HANGUL);
	if (sKey & 0x01)
	{
		g_ActionInput.bHangul = true;
	}
	return true;
}


bool HInput::Render()
{
	char szBuffer[256] = { 0, };
	sprintf_s(szBuffer, "\nW=%d, A=%d, D=%d",
		(g_ActionInput.bWKey) ? 1 : 0,
		(g_ActionInput.bAKey) ? 1 : 0,
		(g_ActionInput.bDKey) ? 1 : 0);
	OutputDebugStringA(szBuffer);

	WCHAR			m_MouseBuffer[256];
	ZeroMemory(m_MouseBuffer, sizeof(WCHAR) * 256);

	GetCursorPos(&g_MousePos);
	ScreenToClient(g_hwnd, &g_MousePos);
	_stprintf_s(m_MouseBuffer, L"xÁÂÇ¥ : %d, yÁÂÇ¥ : %d", g_MousePos.x, g_MousePos.y);
	TextOut(g_hOffScreenDC, 50, 50, m_MouseBuffer, wcslen(m_MouseBuffer - 1));

	// InvalidateRect(g_hwnd, NULL, TRUE);
	return true;
}

bool HInput::Release()
{
	DeleteObject(m_MouseBuffer);
	return true;
}

HInput::HInput()
{

}
HInput::~HInput()
{

}