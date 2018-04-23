#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"


// ----------------------------- forward declaration -----------------------------

namespace Ogre {

class ParticleSystem;

}

namespace Engine {

// ========================== class ParticleComponent ===========================

class ParticleComponent : public RenderComponent
{
public:
						ParticleComponent (const std::string& name, const std::string& partName);

	void				PostInit (GameObject* obj) override;
	void				Destroy () override;

	DLL_EXPORT void		SetEnabled (bool enable);

private:
	static unsigned int		s_instanceCount;

	Ogre::ParticleSystem*	m_pParticleSys;
	std::string				m_particleName;
};

}	// namespace Engine

#endif	// #ifndef PARTICLE_COMPONENT_H
