#ifndef LIGHT_CREATOR_H
#define LIGHT_CREATOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "ComponentCreator.h"
#include "LightComponent.h"
#include <memory>


namespace Engine {

// ============================= class LightCreator =============================

class LightCreator : public ComponentCreator
{
public:
	void Create (GameObject* gameObject) override;
	void ApplyInitData () override;

	void						SetInitData (const LightComponent::InitData& initData);
	void						SetInitData (LightComponent::InitData&& initData);
	LightComponent::InitData&	GetInitData ();

private:
	LightComponent::InitData		m_initData;
	std::shared_ptr<LightComponent> m_managedComponent;
};

}	// namespace Engine 

#endif	// #ifndef LIGHT_CREATOR_H
