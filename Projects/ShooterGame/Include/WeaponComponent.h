#pragma once
#include "MeshComponent.h"

using namespace Engine;

class WeaponComponent : public Component
{
	
public:
	WeaponComponent(const std::string& name);
	~WeaponComponent();

	virtual void Start() override;
	virtual void PreUpdate(float t, float dt) override;
};