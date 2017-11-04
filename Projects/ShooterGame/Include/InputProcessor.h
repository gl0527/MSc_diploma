#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"

using namespace Engine;

// ============================ class InputProcessor =============================

class InputProcessor : public TagProcessor
{
public:
	InputProcessor() : TagProcessor("inputcomponent") {}
	virtual bool ProcessXMLTag(TiXmlElement* elem) override;
};

#endif	// #ifndef INPUT_PROCESSOR_H
