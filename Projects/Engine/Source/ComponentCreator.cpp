#include "ComponentCreator.h"

namespace Engine {

void ComponentCreator::SetInitData (std::shared_ptr<Component::InitData> pInitData)
{
	m_pInitData = pInitData;
}


std::shared_ptr<Component::InitData>& ComponentCreator::GetInitData ()
{
	return m_pInitData;
}


void ComponentCreator::ApplyInitData ()
{
	if (m_pComponent != nullptr)
		m_pComponent->ApplyInitData (m_pInitData);
}

}	// namespace Engine
