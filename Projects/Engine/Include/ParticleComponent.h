#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"


namespace Engine
{

// ========================== class ParticleComponent ===========================

	class DLL_SPEC ParticleComponent : public RenderComponent
	{
	public:
		ParticleComponent(const std::string& name, const std::string& partName);
		virtual ~ParticleComponent();

		virtual void Init(GameObject* obj) override;
		virtual void Destroy() override;

	private:
		static unsigned int instanceCount;
		Ogre::ParticleSystem* partSys;
		std::string particle;
	};

}	// namespace Engine

#endif	// #ifndef PARTICLE_COMPONENT_H
