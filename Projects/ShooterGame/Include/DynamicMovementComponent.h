#ifndef DYNAMIC_MOVEMENT_COMPONENT_H
#define DYNAMIC_MOVEMENT_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "PhysicsComponent.h"
#include "InputHandler.h"
#include "SoldierAnimComponent.h"
#include <memory>

using namespace Engine;

// ======================= class DynamicMovementComponent =======================

class DynamicMovementComponent : public Component
{
	float moveSpeed;
	std::weak_ptr<PhysicsComponent> ownerPhysics;
	std::weak_ptr<SoldierAnimComponent> ownerSoldierComp;
public:
	DynamicMovementComponent(const std::string& name);
	~DynamicMovementComponent();
	
	virtual void Start() override;
	virtual void PreUpdate(float t, float dt) override;

	void setMoveSpeed(float speed) { moveSpeed = speed; }
};

#endif	// #ifndef DYNAMIC_MOVEMENT_COMPONENT_H