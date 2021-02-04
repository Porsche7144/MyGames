#include "HFiniteState.h"


void HFiniteState::AddTransition(DWORD dwEvent, DWORD dwOutputState)
{
	m_dwMapList[dwEvent] = dwOutputState;
}
