#ifndef CAMERA_PROCESSOR_H
#define CAMERA_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"


namespace Engine {
namespace XML {

// =========================== class CameraProcessor ============================

class CameraProcessor : public TagProcessor
{
public:
	CameraProcessor () : TagProcessor ("cameracomponent") {}
	virtual bool ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef CAMERA_PROCESSOR_H
