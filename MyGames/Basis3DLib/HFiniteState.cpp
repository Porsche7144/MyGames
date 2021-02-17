#include "HFiniteState.h"

void HFiniteState::AddTransition(DWORD dwEvent, DWORD dwOutputState)
{
	m_mapList[dwEvent] = dwOutputState;
}
