#pragma once

#ifndef MESH_PROCESSOR_H
#define MESH_PROCESSOR_H

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"


namespace Engine {
namespace XML {

// ============================= class MeshProcessor =============================

class MeshProcessor : public TagProcessor
{
public:
					MeshProcessor () : TagProcessor ("meshcomponent") {}

	virtual bool	ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef MESH_PROCESSOR_H
