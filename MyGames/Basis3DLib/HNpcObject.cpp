#include "HNpcObject.h"
#include "HFiniteStateMachine.h"
#include "HScene.h"
#include "HObjectMgr.h"

HObject* HNpcObject::Clone()
{
	return new HNpcObject(*this);
}
void HNpcObject::FSM()
{
	m_pActionList.push_back(new HStandState(this));
	m_pActionList.push_back(new HMoveState(this));
	m_pActionList.push_back(new HAttackState(this));
	m_pAction = m_pActionList[0];
	
	m_fAttackTimer = 1.0f;
	m_pProjectile = (HEffect*)g_ObjectMgr.GetPtr(L"rtProjectile");

}
void HNpcObject::SetTransition(DWORD dwEvent)
{
	DWORD dwOutput = g_Fsm.GetTransition(m_pAction->m_dwState, 
									dwEvent);
	m_pAction = m_pActionList[dwOutput];
}
bool HNpcObject::Frame()
{
	if (m_bDead == true) return true;
	m_pAction->Process(HScene::m_pGamePlayer);	
	return true;
}
bool HNpcObject::Render(ID3D11DeviceContext* pd3dContext)
{
	HObject::Render(pd3dContext);
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
bool HNpcObject::Release()
{
	HObject::Release();
	for (int iAction = 0; iAction < m_pActionList.size(); iAction++)
	{
		delete m_pActionList[iAction];
	}
	m_pActionList.clear();
	return true;
}
