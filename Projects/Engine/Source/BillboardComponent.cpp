#include "BillboardComponent.h"
#include "GameObject.h"
#include "Ogre.h"


namespace Engine {

unsigned int BillboardComponent::instanceCount = 0;

BillboardComponent::BillboardComponent (const std::string& bbName)
	: RenderComponent (bbName),
	billboardSet (nullptr)
{
}


BillboardComponent::~BillboardComponent ()
{
}


void BillboardComponent::PostInit (GameObject* obj)
{
	billboardSet = m_pSceneManager->createBillboardSet (obj->GetName () + Ogre::StringConverter::toString (instanceCount++));
	m_pObject = billboardSet;
	RenderComponent::PostInit (obj);
}


void BillboardComponent::Destroy ()
{
	m_pSceneManager->destroyBillboardSet (billboardSet->getName ());
}

}	// namespace Engine
