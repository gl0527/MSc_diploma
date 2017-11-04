#ifndef STATEABLE_H
#define STATEABLE_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include <memory>
#include "Component.h"


namespace Engine
{

// ----------------------------- forward declaration -----------------------------

class IState;

// ============================== class Stateable ===============================

class Stateable : public Component
{
public:
	using Component::Component;
	virtual DLL_EXPORT ~Stateable ();

	void DLL_EXPORT SetCurrentState (std::shared_ptr<IState> newState);
	void DLL_EXPORT PreUpdate (float t, float dt) override;

protected:
	std::shared_ptr<IState> m_pCurrentState;
};

}	// namespace Engine

#endif	// #ifndef STATEABLE_H
