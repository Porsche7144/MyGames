#pragma once
#include "HFiniteState.h"
class HFiniteStateMachine : public HSingleton<HFiniteStateMachine>
{
	friend class HSingleton<HFiniteStateMachine>;
private:
	//        STAND   STAND-CLASS
	//        �ε���  ����Ŭ����
	std::map< DWORD, HFiniteState*> m_mapState;
public:
	void    AddStateTrasition(  DWORD dwState, 
								DWORD dwEvent, 
								DWORD dwOutputState);
	DWORD	GetTransition(DWORD dwState, DWORD dwEvent);
	HFiniteStateMachine() = default;
	~HFiniteStateMachine();
};

#define g_Fsm HSingleton<HFiniteStateMachine>::GetInstance()