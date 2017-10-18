#ifndef EVENT_ID_H
#define EVENT_ID_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"

namespace Engine
{

// =============================== class EventID ================================

class EventID
{
public:
	std::string description;
	Component* source;
};

}	// namespace Engine

#endif	// #ifndef EVENT_ID_H
