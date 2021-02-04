#include "HWrite.h"


void   HWrite::push(HMsg msg)
{
	if (m_strList.size() > 10)
	{
		m_strList.pop_back();
	}
	m_strList.insert(
		m_strList.begin(),
		msg);
}

// �ʱ�ȭ �� �غ� �۾�
bool HWrite::Init()
{
	// ��Ʈ �ʱ�ȭ
	m_Font = CreateFont(25, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, _T("����"));
	SelectObject(g_hOffScreenDC, m_Font);
	return true;
}
// ������ ������ ����۾�
bool HWrite::Frame()
{
	return true;
}
// ������ ������ ��� (��ο�, ������)
bool HWrite::Render()
{
		//// ��Ʈ����
		//m_Font = CreateFont(20, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, _T("����"));
		//SelectObject(g_hOffScreenDC, m_Font);
		SetTextColor(g_hOffScreenDC, RGB(255, 0, 0)); // �ؽ�Ʈ �÷�
		SetBkColor(g_hOffScreenDC, RGB(0, 0, 0)); // �ؽ�Ʈ ��� �÷�
		// SetBkMode(g_hOffScreenDC, TRANSPARENT); // ����
	return true;

}
// �Ҹ� �� ���� ��� ����
bool HWrite::Release()
{
	ReleaseDC(g_hwnd, hdc);

	SelectObject(hdc, m_oldFont);
	DeleteObject(m_Font);
	return true;
}