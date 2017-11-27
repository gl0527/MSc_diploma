#include "LightCreator.h"
#include "GameObject.h"


namespace Engine {

void LightCreator::SetInitData (const LightComponent::InitData& initData)
{
	m_initData = initData;
}


void LightCreator::SetInitData (LightComponent::InitData&& initData)
{
	m_initData = initData;
}


LightComponent::InitData& LightCreator::GetInitData ()
{
	return m_initData;
}


void LightCreator::Create (GameObject* gameObject)
{
	if (m_managedComponent == nullptr) {
		m_managedComponent.reset (new LightComponent (m_initData));
		gameObject->AddComponent (m_managedComponent);
	}
}


void LightCreator::ApplyInitData ()
{
	if (m_managedComponent != nullptr)
		m_managedComponent->ApplyCreationData (m_initData);
}

}	// namespace Engine
