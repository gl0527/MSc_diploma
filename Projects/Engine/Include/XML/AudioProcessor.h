#pragma once

#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"


namespace Engine {
namespace XML {

// ============================ class AudioProcessor ============================

class AudioProcessor : public TagProcessor
{
public:
					AudioProcessor () : TagProcessor ("audiocomponent") {}

	virtual bool	ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef AUDIO_PROCESSOR_H
