#ifndef TRANSFORM_PROCESSOR_H
#define TRANSFORM_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"


namespace Engine {
namespace XML {

// ========================== class TransformProcessor ==========================

class TransformProcessor : public TagProcessor
{
public:
	TransformProcessor () : TagProcessor ("transformcomponent") {}
	virtual bool ProcessXMLTag (TiXmlElement* elem) override;

private:
	virtual void AddToParentObject (TiXmlElement* elem, const std::shared_ptr<Component>& component) override;
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef TRANSFORM_PROCESSOR_H
