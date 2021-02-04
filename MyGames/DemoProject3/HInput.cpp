#include "HInput.h"

HInputKey	g_ActionInput;

DWORD HInput::GetKey(DWORD dKey)
{
	// ���� Ű���¸� �˾ƿ��� �Լ�
	SHORT sKey = GetAsyncKeyState(dKey);


	if (sKey & 0x8000) // 1000 0000  ���ڸ��� 1�̸� ��������, 0�̸� �� ����
	{
		if (m_dwKeyState[dKey] == KEY_FREE || m_dwKeyState[dKey] == KEY_UP)
		{
			m_dwKeyState[dKey] = KEY_PUSH;
		}
		else
		{
			m_dwKeyState[dKey] = KEY_HOLD;
		}
	}
	else
	{
		if (m_dwKeyState[dKey] == KEY_PUSH || m_dwKeyState[dKey] == KEY_HOLD)
		{
			m_dwKeyState[dKey] = KEY_UP;
		}
		else
		{
			m_dwKeyState[dKey] = KEY_FREE;
		}
	}
	return m_dwKeyState[dKey];
}

// �ʱ�ȭ
bool HInput::init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	return true;
}

bool HInput::Frame()
{
	// ���콺 ��ǥ
	GetCursorPos(&m_MousePos);
	ScreenToClient(g_hwnd, &m_MousePos);

	g_ActionInput.bWKey = GetKey('W');
	g_ActionInput.bAKey = GetKey('A');
	g_ActionInput.bDKey = GetKey('D');
	g_ActionInput.bSKey = GetKey('S');
	g_ActionInput.bSpaceKey = GetKey(VK_SPACE);
	g_ActionInput.bExit = GetKey(VK_ESCAPE);
	
	g_ActionInput.bLeftClick = GetKey(VK_LBUTTON);
	g_ActionInput.bRightClick = GetKey(VK_RBUTTON);
	g_ActionInput.bMiddleClick = GetKey(VK_MBUTTON);

	return true;
}


bool HInput::Render()
{
	HDC hdc;
	PAINTSTRUCT ps;
	WCHAR m_Buffer[25];
	hdc = BeginPaint(g_hwnd, &ps);


	wsprintf(m_Buffer, L"x��ǥ : %d, y��ǥ : %d", m_MousePos.x, m_MousePos.y);
	TextOut(hdc, 10, 10, m_Buffer, wcslen(m_Buffer));
	
	ZeroMemory(m_csBuffer, sizeof(TCHAR) * 256);
	m_strList.clear();

	wstring strData0;
	wstring strData1;
	wstring strData2;

	if (m_MouseState[0] == TRUE)
	{
		strData0 = L"m_MouseState[0]";
		m_strList.push_back(strData0);
	}
	if (m_MouseState[1] == TRUE)
	{
		strData1 = L"m_MouseState[1]";
		m_strList.push_back(strData1);
	}
	if (m_MouseState[0] == TRUE)
	{
		strData2 = L"m_MouseState[2]";
		m_strList.push_back(strData2);
	}

	DeleteObject(m_Buffer);
	return true;
}

bool HInput::Release()
{
	return true;
}

HInput::HInput()
{

}
HInput::~HInput()
{

}