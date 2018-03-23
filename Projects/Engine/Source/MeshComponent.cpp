#include "MeshComponent.h"
#include "GameObject.h"
#include "OgreStringConverter.h"
#include "OgreSceneManager.h"
#include "OgreEntity.h"


namespace Engine {

unsigned int MeshComponent::s_instanceCount = 0;

MeshComponent::Descriptor::Descriptor ()
	: entityName (""),
	meshFileName (""),
	materialName ("")
{
}


MeshComponent::MeshComponent (const std::string& eName, const std::string& mName)
	: RenderComponent (eName),
	m_pEntity (nullptr),
	m_meshName (mName)
{
}


MeshComponent::MeshComponent (const Descriptor& desc)
	: MeshComponent (desc.entityName, desc.meshFileName)
{
}


MeshComponent::~MeshComponent ()
{
}


void MeshComponent::PostInit (GameObject* obj)
{
	m_pEntity = m_pSceneManager->createEntity (obj->GetName () + Ogre::StringConverter::toString (s_instanceCount++), m_meshName);
	m_pObject = m_pEntity;
	RenderComponent::PostInit (obj);
}


void MeshComponent::Destroy ()
{
	if (m_pSceneManager)
		m_pSceneManager->destroyEntity (m_pEntity->getName ());
}


void MeshComponent::ApplyDescriptor (const Descriptor& desc)
{
	if (desc.materialName != std::string (""))
		SetMaterial (desc.materialName);
}


void MeshComponent::SetMaterial (const std::string& matName)
{
	if (m_pEntity)
		m_pEntity->setMaterialName (matName);
}


Ogre::Entity* MeshComponent::GetEntity () const
{
	return m_pEntity;
}

}	// namespace Engine
