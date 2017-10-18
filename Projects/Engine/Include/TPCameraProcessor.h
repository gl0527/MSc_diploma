#ifndef TP_CAMERA_PROCESSOR_H
#define TP_CAMERA_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"


namespace Engine
{

// ========================== class TPCameraProcessor ==========================

class TPCameraProcessor : public TagProcessor
{
public:
	TPCameraProcessor () : TagProcessor ("tpcameracomponent") {}
	virtual bool ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace Engine

#endif	// #ifndef TP_CAMERA_PROCESSOR_H
