#ifndef DYNAMIC_MOVEMENT_PROCESSOR_H
#define DYNAMIC_MOVEMENT_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"


using namespace Engine;

// ======================= class DynamicMovementProcessor =======================

class DynamicMovementProcessor : public XML::TagProcessor
{
public:
	DynamicMovementProcessor() : XML::TagProcessor("dynamicmovementcomponent") {}
	virtual bool ProcessXMLTag(TiXmlElement* elem) override;
};

#endif	// #ifndef DYNAMIC_MOVEMENT_PROCESSOR_H
