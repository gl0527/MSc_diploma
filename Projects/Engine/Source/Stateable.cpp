#include "Stateable.h"
#include "IState.h"


namespace Engine
{

Stateable::Stateable ()
{
}


Stateable::~Stateable ()
{
}


void Stateable::SetCurrentState (IState* newState)
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit (this);
	
	if (newState != nullptr)
		m_pCurrentState.reset (newState);
	
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Enter (this);
}


void Stateable::MakeStep (float t, float dt)
{
	if (m_pCurrentState != nullptr)	// TODO es mi van akkor, ha az execute-ot csak egyszer szeretnem meghivni? (pl.: PlayAnimation ())
		m_pCurrentState->Execute (this, t, dt);
}

}
