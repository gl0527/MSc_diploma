#pragma once

#ifndef ISTATE_H
#define ISTATE_H

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include "NonCopyable.h"
#include <string>


namespace Engine {

// ----------------------------- forward declaration -----------------------------

class Stateable;

// ================================ class IState ================================

class IState : public NonCopyable
{
public:
	explicit DLL_EXPORT			IState (const std::string& name);
	virtual DLL_EXPORT			~IState ();

	virtual DLL_EXPORT void		Enter (Stateable* stateable) = 0;
	virtual DLL_EXPORT void		Execute (Stateable* stateable, float t, float dt) = 0;
	virtual DLL_EXPORT void		Exit (Stateable* stateable) = 0;

	void						Enable ();
	void						Disable ();

protected:
	const std::string	m_name;
	volatile bool		m_isEnabled;
};

}	// namespace Engine

#endif	// #ifndef ISTATE_H
