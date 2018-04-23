#include "ParticleComponent.h"
#include "GameObject.h"
#include "OgreStringConverter.h"
#include "OgreSceneManager.h"
#include "OgreParticleSystem.h"


namespace Engine {

unsigned int ParticleComponent::s_instanceCount = 0;

ParticleComponent::ParticleComponent (const std::string& name, const std::string& partName)
	: RenderComponent (name),
	m_pParticleSys (nullptr),
	m_particleName (partName)
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


void ParticleComponent::SetEnabled (bool enable)
{
	if (m_pParticleSys == nullptr)
		return;

	m_pParticleSys->setEmitting (enable);
}

}	// namespace Engine
