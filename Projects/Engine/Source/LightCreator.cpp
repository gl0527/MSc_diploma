#include "LightCreator.h"

namespace Engine {

void LightCreator::SetCreationData (const LightComponent::InitData& creationData)
{
	m_creationData = creationData;
}


void LightCreator::SetCreationData (LightComponent::InitData&& creationData)
{
	m_creationData = creationData;
}


LightComponent::InitData& LightCreator::GetCreationData ()
{
	return m_creationData;
}


void LightCreator::ApplyCreationData ()
{
	if (m_managedComponent != nullptr)
		m_managedComponent->ApplyCreationData (m_creationData);
}

}	// namespace Engine
