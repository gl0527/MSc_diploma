#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include <map>
#include "EventHandler.h"
#include "EventID.h"


namespace Engine
{

// ============================ class EventManager =============================

class EventManager
{
public:
	EventManager ();
	~EventManager ();

	void handleEvents ();
	void bind (EventID* evtID, EventHandler* evtHandler);
	void unbind (EventID* evtID, EventHandler* evtHandler);

private:
	std::map<EventID*, EventHandler*> handlers;
};

}	// namespace Engine

#endif	// #ifndef EVENT_MANAGER_H
