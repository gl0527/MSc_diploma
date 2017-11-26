#include "Stateable.h"
#include "IState.h"


namespace Engine {

Stateable::~Stateable ()
{
	//m_pCurrentState = nullptr;
}


void Stateable::SetCurrentState (std::shared_ptr<IState> newState)
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit (this);

	if (newState != nullptr)
		m_pCurrentState = newState;

	if (m_pCurrentState != nullptr)
		m_pCurrentState->Enter (this);
}


void Stateable::PreUpdate (float t, float dt)
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Execute (this, t, dt);
}

}
