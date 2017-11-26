#ifndef STATEABLE_H
#define STATEABLE_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include <memory>
#include "Component.h"


namespace Engine {

// ----------------------------- forward declaration -----------------------------

class IState;

// ============================== class Stateable ===============================

class Stateable : public Component
{
public:
	using Component::Component;
	virtual DLL_EXPORT ~Stateable ();

	DLL_EXPORT void SetCurrentState (std::shared_ptr<IState> newState);
	DLL_EXPORT void PreUpdate (float t, float dt) override;

protected:
	std::shared_ptr<IState> m_pCurrentState;
};

}	// namespace Engine

#endif	// #ifndef STATEABLE_H
