#ifndef DYNAMIC_MOVEMENT_COMPONENT_H
#define DYNAMIC_MOVEMENT_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"


namespace Engine {

class PhysicsComponent;

}	// namespace Engine

//class PlayerDataComponent;

using namespace Engine;

// ======================= class DynamicMovementComponent =======================

class DynamicMovementComponent : public Component
{
public:
	explicit		DynamicMovementComponent(const std::string& name);
	
	void			Start() override;
	void			PreUpdate(float t, float dt) override;

	void			SetMoveSpeed(float moveSpeed);
	void			SetTurnSpeed (float turnSpeed);

private:
	float									m_moveSpeed;
	float									m_turnSpeed;
	std::shared_ptr<PhysicsComponent>		m_pOwnerPhysics;
	//std::shared_ptr<PlayerDataComponent>	m_pOwnerData;


	void			OnCollisionWithWeapon (PhysicsComponent* other);
	void			OnCollisionWithTable (PhysicsComponent* other);
};

#endif	// #ifndef DYNAMIC_MOVEMENT_COMPONENT_H
