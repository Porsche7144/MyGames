#include "HNpcObject.h"
#include "HFiniteStateMachine.h"
#include "HScene.h"
#include "HObjectManager.h"

void HNpcObject::FSM()
{
	m_pActionList.push_back(new HMoveState(this));
	m_pActionList.push_back(new HAttackState(this));
	m_pActionList.push_back(new HStandState(this));
	m_pAction = m_pActionList[0];

	m_fTmpTimer = 1.0f;
	m_pProjectile = (HEffect*)g_ObjMgr.GetPtr(L"rtProjectile");

}
void HNpcObject::SetTransition(DWORD dwEvent)
{
	DWORD dwOutput = g_FSM.GetTransition(m_pAction->m_dwState,
		dwEvent);
	m_pAction = m_pActionList[dwOutput];
}
bool HNpcObject::Frame()
{
	if (m_bDead == true) return true;
	m_pAction->Process(HScene::m_pGamePlayer);
	return true;
}
bool HNpcObject::Render()
{
	HObject::Render();
	return true;
}

bool HNpcObject::Damage()
{
	return false;
}
bool HNpcObject::Dead()
{
	return true;
}
