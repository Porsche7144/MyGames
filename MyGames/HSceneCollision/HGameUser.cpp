#include "HGameUser.h"

bool HGameUser::Frame()
{
	if (g_ActionInput.bWKey)
	{
		m_ptPos.y -= m_fSpeed * g_fSecondPerFrame;
	}
	if (g_ActionInput.bAKey)
	{
		m_ptPos.x -= m_fSpeed * g_fSecondPerFrame;
	}
	if (g_ActionInput.bDKey)
	{
		m_ptPos.x += m_fSpeed * g_fSecondPerFrame;
	}
	if (g_ActionInput.bSKey)
	{
		m_ptPos.y += m_fSpeed * g_fSecondPerFrame;
	}
	SetPosition(m_ptPos);
	return true;
}