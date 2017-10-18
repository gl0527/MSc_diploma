#ifndef PARTICLE_PROCESSOR_H
#define PARTICLE_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"


namespace Engine
{

// ========================== class ParticleProcessor ===========================

	class ParticleProcessor : public TagProcessor
	{
	public:
		ParticleProcessor() : TagProcessor("particlecomponent") {}
		virtual bool ProcessXMLTag(TiXmlElement* elem) override;
	};

}	// namespace Engine

#endif	// #ifndef PARTICLE_PROCESSOR_H
