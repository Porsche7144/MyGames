#include "HStandState.h"


void HStandState::Process(HObject* pPlayer)
{
	if (rand() % 1000 < 3)
	{
		m_pOwner->SetTransition(EVENT_TIMEMOVE);
	}

	HPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = ptDist.Length();
	if (fDistance < 200)
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}
}

HStandState::HStandState(HObject* owner) : HEnemyState(owner)
{
	m_dwState = STATE_STAND;
}
