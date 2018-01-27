#ifndef PARTICLE_PROCESSOR_H
#define PARTICLE_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"


namespace Engine {
namespace XML {

// ========================== class ParticleProcessor ===========================

class ParticleProcessor : public TagProcessor
{
public:
					ParticleProcessor () : TagProcessor ("particlecomponent") {}

	virtual bool	ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef PARTICLE_PROCESSOR_H
