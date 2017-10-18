#ifndef STATEABLE_H
#define STATEABLE_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include <memory>


namespace Engine
{

// ----------------------------- forward declaration -----------------------------

class IState;

// ============================== class Stateable ===============================

class DLL_SPEC Stateable
{
public:
	Stateable ();
	virtual ~Stateable ();

	void SetCurrentState (IState* newState);
	void MakeStep (float t, float dt);

protected:
	std::shared_ptr<IState> m_pCurrentState;
};

}	// namespace Engine

#endif	// #ifndef STATEABLE_H
