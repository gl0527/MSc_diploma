#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"

using namespace Engine;

// ============================ class InputProcessor =============================

class InputProcessor : public XML::TagProcessor
{
public:
	InputProcessor() : XML::TagProcessor("inputcomponent") {}
	virtual bool ProcessXMLTag(TiXmlElement* elem) override;
};

#endif	// #ifndef INPUT_PROCESSOR_H
