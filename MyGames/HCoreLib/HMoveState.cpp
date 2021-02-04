#include "HMoveState.h"


void HMoveState::Process(HObject* pPlayer)
{
	if (m_pOwner->m_ptPos.x < g_rt.left)
	{
		m_pOwner->m_ptDirection.x *= -1.0f;
		m_pOwner->m_ptPos.x = g_rt.left;
	}
	if (m_pOwner->m_ptPos.y < g_rt.top)
	{
		m_pOwner->m_ptDirection.y *= -1.0f;
		m_pOwner->m_ptPos.y = g_rt.top;
	}
	if (m_pOwner->m_ptPos.x > g_rt.right)
	{
		m_pOwner->m_ptDirection.x *= -1.0f;
		m_pOwner->m_ptPos.x = g_rt.right;
	}
	if (m_pOwner->m_ptPos.y > g_rt.bottom)
	{
		m_pOwner->m_ptDirection.y *= -1.0f;
		m_pOwner->m_ptPos.y = g_rt.bottom;
	}
	m_pOwner->m_ptPos.x += m_pOwner->m_ptDirection.x *
		m_pOwner->m_fSpeed *
		(g_fSecondPerFrame + 1) * 0.002;
	m_pOwner->m_ptPos.y += m_pOwner->m_ptDirection.y *
		m_pOwner->m_fSpeed *
		(g_fSecondPerFrame + 1) * 0.002;

	m_pOwner->SetPosition(m_pOwner->m_ptPos);

	if (rand() % 10000 < 3)
	{
		m_pOwner->SetTransition(EVENT_STOPMOVE);
	}
	HPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = ptDist.Length();
	if (fDistance < 200)
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}
}
HMoveState::HMoveState(HObject* owner) : HEnemyState(owner)
{
	m_dwState = STATE_MOVE;
}
