#ifndef MESH_PROCESSOR_H
#define MESH_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"


namespace Engine
{

// ============================= class MeshProcessor =============================

	class MeshProcessor : public TagProcessor
	{
	public:
		MeshProcessor() : TagProcessor("meshcomponent") {}
		virtual bool ProcessXMLTag(TiXmlElement* elem) override;
	};

}	// namespace Engine

#endif	// #ifndef MESH_PROCESSOR_H
