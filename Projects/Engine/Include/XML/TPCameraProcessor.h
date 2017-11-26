#ifndef TP_CAMERA_PROCESSOR_H
#define TP_CAMERA_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"


namespace Engine {
namespace XML {

// ========================== class TPCameraProcessor ==========================

class TPCameraProcessor : public TagProcessor
{
public:
	TPCameraProcessor () : TagProcessor ("tpcameracomponent") {}
	virtual bool ProcessXMLTag (TiXmlElement* elem) override;
};

}
}	// namespace Engine

#endif	// #ifndef TP_CAMERA_PROCESSOR_H
