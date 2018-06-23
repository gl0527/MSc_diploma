#pragma once

#ifndef PHYSICS_PROCESSOR_H
#define PHYSICS_PROCESSOR_H

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"


namespace Engine {

// ----------------------------- forward declaration -----------------------------

class PhysicsComponent;

namespace XML {

// =========================== class PhysicsProcessor ===========================

class PhysicsProcessor : public TagProcessor
{
public:
			PhysicsProcessor () : TagProcessor ("physicscomponent") {}

	bool	ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef PHYSICS_PROCESSOR_H
