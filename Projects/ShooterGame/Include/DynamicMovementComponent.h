#ifndef DYNAMIC_MOVEMENT_COMPONENT_H
#define DYNAMIC_MOVEMENT_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"


namespace Engine {

class PhysicsComponent;

}

using namespace Engine;

// ======================= class DynamicMovementComponent =======================

class DynamicMovementComponent : public Component
{
public:
					DynamicMovementComponent(const std::string& name);
	
	virtual void	Start() override;
	virtual void	PreUpdate(float t, float dt) override;

	void			SetMoveSpeed(float moveSpeed);
	void			SetTurnSpeed (float turnSpeed);

private:
	float								m_moveSpeed;
	float								m_turnSpeed;
	std::shared_ptr<PhysicsComponent>	m_pOwnerPhysics;

	void			OnCollisionWithWeapon (PhysicsComponent* other);
};

#endif	// #ifndef DYNAMIC_MOVEMENT_COMPONENT_H
