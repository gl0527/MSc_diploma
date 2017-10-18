#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include <vector>
#include "EventID.h"

namespace Engine
{

// ============================= class EventHandler =============================

class EventHandler
{
public:
	EventHandler ();
	~EventHandler ();

	virtual void handleEvent (EventID* evtID) = 0;
};

}	// namespace Engine

#endif	// #ifndef EVENT_HANDLER_H
