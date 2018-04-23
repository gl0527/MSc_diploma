#ifndef	ENVIRONMENT_DATA_PROCESSOR_H
#define ENVIRONMENT_DATA_PROCESSOR_H

#pragma once


#include "TagProcessor.h"

namespace Engine {
namespace XML {

class EnvironmentDataProcessor : public TagProcessor
{
public:
			EnvironmentDataProcessor () : TagProcessor ("environment") {}

	bool	ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace XML
}	// namespace Engine


#endif	// ENVIRONMENT_DATA_PROCESSOR_H
