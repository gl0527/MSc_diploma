#include "ParticleComponent.h"
#include "GameObject.h"
#include "Ogre.h"


namespace Engine {

unsigned int ParticleComponent::s_instanceCount = 0;

ParticleComponent::ParticleComponent (const std::string& name, const std::string& partName)
	: RenderComponent (name),
	m_pParticleSys (nullptr),
	m_particleName (partName)
{
}


ParticleComponent::~ParticleComponent ()
{
}


void ParticleComponent::PostInit (GameObject* obj)
{
	m_pParticleSys = m_pSceneManager->createParticleSystem (obj->GetName () + Ogre::StringConverter::toString (s_instanceCount++), m_particleName);
	m_pObject = m_pParticleSys;
	RenderComponent::PostInit (obj);
}


void ParticleComponent::Destroy ()
{
	m_pSceneManager->destroyParticleSystem (m_pParticleSys->getName ());
}

}	// namespace Engine
