#include "HWrite.h"
#include "HTimer.h"

void   HWrite::push(HMsg msg)
{
	std::lock_guard<std::mutex> lock(m_hMutex);
	if (m_TextList.size() > 10)
	{
		m_TextList.pop_back();
	}
	m_TextList.insert(
			m_TextList.begin(), 
			msg);	
}
bool HWrite::Init()
{	
	m_hFont = CreateFont( 20, 0, 0, FW_BOLD, 0,0,0,0,DEFAULT_CHARSET, 0,0,0, 0,	L"°íµñ"	);
	SelectObject(g_hOffScreenDC, m_hFont);
	return true;
}
bool HWrite::Frame()
{
	return true;
}
bool HWrite::Render()
{	
	std::lock_guard<std::mutex> lock(m_hMutex);
	if (g_hOffScreenDC != NULL)
	{
		//RECT rect;
		//GetClientRect(g_hWnd, &rect);
		SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(g_hOffScreenDC, RGB(255, 255, 255));
		SetBkMode(g_hOffScreenDC, TRANSPARENT);
		for (int iMsg = 0; iMsg < m_TextList.size(); iMsg++)
		{
			m_TextList[iMsg].rt.top = 25+iMsg * 25;
			DrawText(g_hOffScreenDC,
				m_TextList[iMsg].msg.c_str(),
				-1,
				&m_TextList[iMsg].rt,
				DT_LEFT);
			/*TextOut(g_hOffScreenDC, 0, 0,
				g_Timer.m_szBuffer,
				_tcslen(g_Timer.m_szBuffer)-1);*/
		}
	}	
	return true;
}
void HWrite::Draw(int x, int y, wstring msg, DWORD color)
{
	POINT p = { x, y };
	Draw(p, msg, color);
}
void HWrite::Draw(POINT pos, wstring msg, DWORD color)
{
	std::lock_guard<std::mutex> lock(m_hMutex);
	if (g_hOffScreenDC != NULL)
	{
		SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(g_hOffScreenDC, color);
		SetBkMode(g_hOffScreenDC, TRANSPARENT);
		TextOut(g_hOffScreenDC, pos.x, pos.y,msg.c_str(),(int)msg.size());
	}
}
bool HWrite::Release()
{
	DeleteObject(m_hFont);	
	return true;
}

HWrite::HWrite()
{

}
HWrite::~HWrite()
{

}