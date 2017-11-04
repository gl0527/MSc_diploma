#ifndef DYNAMIC_MOVEMENT_PROCESSOR_H
#define DYNAMIC_MOVEMENT_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"


using namespace Engine;

// ======================= class DynamicMovementProcessor =======================

class DynamicMovementProcessor : public TagProcessor
{
public:
	DynamicMovementProcessor() : TagProcessor("dynamicmovementcomponent") {}
	virtual bool ProcessXMLTag(TiXmlElement* elem) override;
};

#endif	// #ifndef DYNAMIC_MOVEMENT_PROCESSOR_H
