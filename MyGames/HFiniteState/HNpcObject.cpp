#include "HNpcObject.h"
#include "HScene.h"
#include "HFiniteStateMachine.h"


void HNpcObject::FSM()
{
	m_pActionList.push_back(new HStandState(this));
	m_pActionList.push_back(new HMoveState(this));
	m_pActionList.push_back(new HAttackState(this));
	m_pAction = m_pActionList[0];
}

void HNpcObject::SetTransition(DWORD dwEvent)
{
	DWORD dwOutput = g_FSM.GetTransition(m_pAction->m_dwState, dwEvent);
	m_pAction = m_pActionList[dwOutput];
}

bool HNpcObject::Frame()
{
	if (m_bDead == true) return true;
	m_pAction->Process(HScene::m_pGamePlayer);
	return true;
}