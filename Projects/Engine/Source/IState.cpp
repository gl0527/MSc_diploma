#include "IState.h"


namespace Engine {

IState::IState (const std::string& name)
	: m_name (name),
	m_isEnabled (true)
{
}


IState::~IState ()
{
}


void IState::Enable ()
{
	m_isEnabled = true;
}


void IState::Disable ()
{
	m_isEnabled = false;
}

}	// namespace Engine
