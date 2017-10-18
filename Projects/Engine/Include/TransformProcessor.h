#ifndef TRANSFORM_PROCESSOR_H
#define TRANSFORM_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"


namespace Engine
{

// ========================== class TransformProcessor ==========================

class TransformProcessor : public TagProcessor
{
public:
	TransformProcessor () : TagProcessor ("transformcomponent") {}
	virtual bool ProcessXMLTag (TiXmlElement* elem) override;

private:
	virtual void AddToParentObject (TiXmlElement* elem, const std::shared_ptr<Component>& component) override;
};

}	// namespace Engine

#endif	// #ifndef TRANSFORM_PROCESSOR_H
