#ifndef SYSTEM_H
#define SYSTEM_H

#pragma once


namespace Engine
{

// ============================ class RenderComponent ============================

class System
{
public:
	virtual ~System () {}
	
	virtual bool init () { return true; }
	virtual bool update (float t, float dt) { return true; }
	virtual void destroy () {}
};

}	// namespace Engine

#endif	// #ifndef SYSTEM_H
