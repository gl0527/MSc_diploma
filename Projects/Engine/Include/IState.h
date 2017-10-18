#ifndef ISTATE_H
#define ISTATE_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include <string>


namespace Engine
{

// ----------------------------- forward declaration -----------------------------

class Stateable;

// ================================ class IState ================================

class DLL_SPEC IState
{
public:
	explicit IState (const std::string& name);
	IState (const IState&) = delete;
	virtual ~IState ();

	IState* operator= (const IState&) = delete;

	virtual void Enter (Stateable* stateable) = 0;
	virtual void Execute (Stateable* stateable, float t, float dt) = 0;
	virtual void Exit (Stateable* stateable) = 0;

	void Enable ();
	void Disable ();

protected:
	const std::string	m_name;
	volatile bool		m_isEnabled;
};

}	// namespace Engine

#endif	// #ifndef ISTATE_H
