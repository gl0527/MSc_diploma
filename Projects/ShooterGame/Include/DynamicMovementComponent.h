#pragma once
#include "PhysicsComponent.h"
#include "InputHandler.h"
#include "SoldierComponent.h"
#include <memory>

using namespace Engine;

class DynamicMovementComponent : public Component
{
	float moveSpeed;
	std::weak_ptr<PhysicsComponent> ownerPhysics;
	std::weak_ptr<SoldierComponent> ownerSoldierComp;
public:
	DynamicMovementComponent(const std::string& name);
	~DynamicMovementComponent();
	
	virtual void Start() override;
	virtual void PreUpdate(float t, float dt) override;

	void setMoveSpeed(float speed) { moveSpeed = speed; }
};