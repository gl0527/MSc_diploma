#pragma once

#ifndef LIGHT_PROCESSOR_H
#define LIGHT_PROCESSOR_H

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"

namespace Engine {
namespace XML {

// ============================ class LightProcessor ============================

class LightProcessor : public TagProcessor
{
public:
					LightProcessor () : TagProcessor ("lightcomponent") {}

	virtual bool	ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef LIGHT_PROCESSOR_H
