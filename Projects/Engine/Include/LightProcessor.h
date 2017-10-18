#ifndef LIGHT_PROCESSOR_H
#define LIGHT_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"

namespace Engine
{

// ============================ class LightProcessor ============================

	class LightProcessor : public TagProcessor
	{
	public:
		LightProcessor() : TagProcessor("lightcomponent") {}
		virtual bool ProcessXMLTag(TiXmlElement* elem) override;
	};

}

#endif	// #ifndef LIGHT_PROCESSOR_H
