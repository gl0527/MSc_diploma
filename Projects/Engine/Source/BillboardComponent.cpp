#include "BillboardComponent.h"
#include "GameObject.h"


namespace Engine
{

unsigned int BillboardComponent::instanceCount = 0;

BillboardComponent::BillboardComponent (const std::string& bbName)
	: RenderComponent (bbName),
	billboardSet (nullptr)
{

}


BillboardComponent::~BillboardComponent ()
{
}


void BillboardComponent::Init (GameObject* obj)
{
	billboardSet = m_pSceneManager->createBillboardSet (obj->GetName () + Ogre::StringConverter::toString (instanceCount++));
	m_pObject = billboardSet;
	RenderComponent::Init (obj);
}


void BillboardComponent::Destroy ()
{
	m_pSceneManager->destroyBillboardSet (billboardSet->getName ());
}

}	// namespace Engine
