#ifndef CAMERA_PROCESSOR_H
#define CAMERA_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "TagProcessor.h"


namespace Engine
{

// =========================== class CameraProcessor ============================

class CameraProcessor : public TagProcessor
{
public:
	CameraProcessor () : TagProcessor ("cameracomponent") {}
	virtual bool ProcessXMLTag (TiXmlElement* elem) override;
};

}	// namespace Engine

#endif	// #ifndef CAMERA_PROCESSOR_H
