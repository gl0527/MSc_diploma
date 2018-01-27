#include "BillboardComponent.h"
#include "GameObject.h"
#include "Ogre.h"


namespace Engine {

unsigned int BillboardComponent::s_instanceCount = 0;

BillboardComponent::BillboardComponent (const std::string& bbName)
	: RenderComponent (bbName),
	m_pBillboardSet (nullptr)
{
}


BillboardComponent::~BillboardComponent ()
{
}


void BillboardComponent::PostInit (GameObject* obj)
{
	m_pBillboardSet = m_pSceneManager->createBillboardSet (obj->GetName () + Ogre::StringConverter::toString (s_instanceCount++));
	m_pObject = m_pBillboardSet;
	RenderComponent::PostInit (obj);
}


void BillboardComponent::Destroy ()
{
	m_pSceneManager->destroyBillboardSet (m_pBillboardSet->getName ());
}


Ogre::BillboardSet*	BillboardComponent::GetBillboardSet () const
{
	return m_pBillboardSet;
}

}	// namespace Engine
