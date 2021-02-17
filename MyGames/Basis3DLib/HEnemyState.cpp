#include "HEnemyState.h"
HEnemyState::HEnemyState(HObject* owner)
{
	m_pOwner = owner;
	m_dwState = 0;
	m_fTimer = 0.0f;
	m_fTransitionInterval = 0.0f;
}