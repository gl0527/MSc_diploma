#include "ParticleComponent.h"
#include "GameObject.h"
#include "Ogre.h"


namespace Engine {

unsigned int ParticleComponent::instanceCount = 0;

ParticleComponent::ParticleComponent (const std::string& name, const std::string& partName)
	: RenderComponent (name),
	partSys (nullptr),
	particle (partName)
{
}


ParticleComponent::~ParticleComponent ()
{
}


void ParticleComponent::PostInit (GameObject* obj)
{
	partSys = m_pSceneManager->createParticleSystem (obj->GetName () + Ogre::StringConverter::toString (instanceCount++), particle);
	m_pObject = partSys;
	RenderComponent::PostInit (obj);
}


void ParticleComponent::Destroy ()
{
	m_pSceneManager->destroyParticleSystem (partSys->getName ());
}

}	// namespace Engine
