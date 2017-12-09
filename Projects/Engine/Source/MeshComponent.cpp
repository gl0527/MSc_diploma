#include "MeshComponent.h"
#include "GameObject.h"
#include "Ogre.h"


namespace Engine {

unsigned int MeshComponent::instanceCount = 0;

MeshComponent::Descriptor::Descriptor ()
	: entityName (""),
	meshFileName (""),
	materialName ("")
{
}


MeshComponent::MeshComponent (const std::string& eName, const std::string& mName)
	: RenderComponent (eName),
	entity (nullptr),
	mesh (mName)
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
	entity = m_pSceneManager->createEntity (obj->GetName () + Ogre::StringConverter::toString (instanceCount++), mesh);
	m_pObject = entity;
	RenderComponent::PostInit (obj);
}


void MeshComponent::Destroy ()
{
	if (m_pSceneManager)
		m_pSceneManager->destroyEntity (entity->getName ());
}


void MeshComponent::ApplyDescriptor (const Descriptor& desc)
{
	if (desc.materialName != std::string (""))
		setMaterial (desc.materialName);
}


void MeshComponent::setMaterial (const std::string& matName)
{
	if (entity)
		entity->setMaterialName (matName);
}

}	// namespace Engine
