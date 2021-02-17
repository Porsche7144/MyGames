#include "HFiniteStateMachine.h"

void HFiniteStateMachine::AddStateTrasition(
	DWORD dwState, DWORD dwEvent, DWORD dwOutputState)
{
	HFiniteState* pState = nullptr;
	auto ret = m_mapState.find(dwState);
	if (ret != m_mapState.end())
	{
		pState = ret->second;
	}
	else
	{
		pState = new HFiniteState(dwState);
		m_mapState[dwState] = pState;
	}	
	pState->AddTransition(dwEvent, dwOutputState);
}
DWORD HFiniteStateMachine::GetTransition(DWORD dwState, 
	DWORD dwEvent)
{
	return m_mapState[dwState]->m_mapList[dwEvent];
}
HFiniteStateMachine::~HFiniteStateMachine()
{
	for (auto list : m_mapState)
	{
		delete list.second;
	}
	m_mapState.clear();
}