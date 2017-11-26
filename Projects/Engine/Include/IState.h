#ifndef ISTATE_H
#define ISTATE_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include <string>


namespace Engine {

// ----------------------------- forward declaration -----------------------------

class Stateable;

// ================================ class IState ================================

class IState
{
public:
	explicit DLL_EXPORT IState (const std::string& name);
	IState (const IState&) = delete;
	virtual DLL_EXPORT ~IState ();

	IState* operator= (const IState&) = delete;

	virtual DLL_EXPORT void Enter (Stateable* stateable) = 0;
	virtual DLL_EXPORT void Execute (Stateable* stateable, float t, float dt) = 0;
	virtual DLL_EXPORT void Exit (Stateable* stateable) = 0;

	void Enable ();
	void Disable ();

protected:
	const std::string	m_name;
	volatile bool		m_isEnabled;
};

}	// namespace Engine

#endif	// #ifndef ISTATE_H
