#ifndef COMPONENT_CREATOR_H
#define COMPONENT_CREATOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"


namespace Engine {

// =========================== class ComponentCreator ===========================

class ComponentCreator
{
public:
	void SetInitData (std::shared_ptr<Component::InitData> pInitData);
	std::shared_ptr<Component::InitData>& GetInitData ();

	void ApplyInitData ();

protected:
	std::shared_ptr<Component::InitData>	m_pInitData;
	std::shared_ptr<Component>				m_pComponent;
};

}	// namespace Engine

#endif	// #ifndef COMPONENT_CREATOR_H
