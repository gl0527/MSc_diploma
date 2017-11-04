#ifndef CAMERA_CONTROL_COMPONENT_H
#define CAMERA_CONTROL_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "CameraComponent.h"


using namespace Engine;

namespace Engine
{
class PhysicsSystem;
}

// ======================== class CameraControlComponent ========================

class CameraControlComponent : public Component
{
	float camHeight;
	float targetHeight;
	float camDist;
	float motBlend;
	bool fixed;
	PhysicsSystem* world;
	std::weak_ptr<CameraComponent> ownerCam;
public:
	CameraControlComponent(const std::string& name);
	~CameraControlComponent();

	virtual void Start() override;
	virtual void PostUpdate(float t, float dt) override;
};

#endif	// #ifndef CAMERA_CONTROL_COMPONENT_H
