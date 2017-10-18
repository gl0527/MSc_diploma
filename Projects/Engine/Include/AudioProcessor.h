#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"


namespace Engine
{

// ============================ class AudioProcessor ============================

class AudioProcessor : public TagProcessor
{
public:
	AudioProcessor () : TagProcessor ("audiocomponent") {}
	virtual bool ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace Engine

#endif	// #ifndef AUDIO_PROCESSOR_H
