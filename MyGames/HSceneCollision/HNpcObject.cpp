#include "HNpcObject.h"

bool HNpcObject::Frame()
{
	if (m_bDead == true) return true;

	if (m_ptPos.x < g_rt.left)
	{
		m_ptDirection.x *= -1.0f;
		m_ptPos.x = g_rt.left;
	}
	if (m_ptPos.y < g_rt.top)
	{
		m_ptDirection.y *= -1.0f;
		m_ptPos.y = g_rt.top;
	}
	if (m_ptPos.x > g_rt.right)
	{
		m_ptDirection.x *= -1.0f;
		m_ptPos.x = g_rt.right;
	}
	if (m_ptPos.y > g_rt.bottom)
	{
		m_ptDirection.y *= -1.0f;
		m_ptPos.y = g_rt.bottom;
	}
	m_ptPos.x += m_ptDirection.x *
		m_fSpeed *
		g_fSecondPerFrame;
	m_ptPos.y += m_ptDirection.y *
		m_fSpeed *
		g_fSecondPerFrame;

	SetPosition(m_ptPos);

	return true;
}