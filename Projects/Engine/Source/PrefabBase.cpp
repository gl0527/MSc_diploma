#include "PrefabBase.h"


namespace Engine {

PrefabBase::PrefabBase (bool immediateCreation)
	: m_owner (nullptr),
	m_immediateCreation (immediateCreation)
{
}


void PrefabBase::Init (GameObject* owner)
{
	m_owner = owner;
}


bool PrefabBase::IsCreationImmediate () const
{
	return m_immediateCreation;
}

}	// namespace Engine
