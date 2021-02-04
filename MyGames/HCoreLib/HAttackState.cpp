#include "HAttackState.h"


void HAttackState::Process(HObject* pPlayer)
{
	HPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = ptDist.Length();
	if (fDistance > 200)
	{
		m_pOwner->SetTransition(EVENT_LOSTTARGET);
	}
}

HAttackState::HAttackState(HObject* owner)
: HEnemyState(owner)
{
	m_dwState = STATE_ATTACK;
}
