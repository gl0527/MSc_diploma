#pragma once
#include "TagProcessor.h"

using namespace Engine;

class InputProcessor : public TagProcessor
{
public:
	InputProcessor() : TagProcessor("inputcomponent") {}
	virtual bool ProcessXMLTag(TiXmlElement* elem) override;
};