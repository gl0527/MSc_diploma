#ifndef PHYSICS_PROCESSOR_H
#define PHYSICS_PROCESSOR_H

#pragma once

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
	virtual bool ProcessXMLTag (TiXmlElement* elem) override;

private:
	bool ProcessCollisionShape (PhysicsComponent* comp, TiXmlElement* child);
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef PHYSICS_PROCESSOR_H
