#ifndef LIGHT_CREATOR_H
#define LIGHT_CREATOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "LightComponent.h"
#include <memory>

namespace Engine {

// ============================= class LightCreator =============================

class LightCreator
{
public:
	void							SetCreationData (const LightComponent::InitData& creationData);
	void							SetCreationData (LightComponent::InitData&& creationData);
	LightComponent::InitData&		GetCreationData ();

	template<typename... Args>
	void							Create (Args&&... args);

	void							ApplyCreationData ();

private:
	LightComponent::InitData		m_creationData;
	std::shared_ptr<LightComponent> m_managedComponent;
};


template<typename... Args>
void LightCreator::Create (Args&&... args)
{
	m_managedComponent.reset (new LightComponent (std::forward (args)...));
}

}	// namespace Engine 

#endif	// #ifndef LIGHT_CREATOR_H
