#ifndef TP_CAMERA_COMPONENT_H
#define TP_CAMERA_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "CameraComponent.h"


namespace Engine {

// ----------------------------- forward declaration-----------------------------

class PhysicsSystem;


// ========================== class TPCameraComponent ==========================

class TPCameraComponent : public CameraComponent
{
public:
	TPCameraComponent (const std::string& name, int zDepth);
	~TPCameraComponent ();

	virtual void PostInit (GameObject* object) override;
	virtual void PostUpdate (float t, float dt) override;

	bool isFixed () const { return fixed; }

	void setCameraHeight (float cHeight) { camHeight = cHeight; }
	void setTargetHeight (float tHeight) { targetHeight = tHeight; }
	void setCameraDist (float dist) { camDist = dist; }
	void setMotionBlend (float mBlend) { motBlend = mBlend; }
	void setFixed (bool fix) { fixed = fix; }

private:
	float camHeight;
	float targetHeight;
	float camDist;
	float motBlend;
	bool fixed;
	PhysicsSystem* physicsSys;
};

}	// namespace Engine

#endif	// #ifndef TP_CAMERA_COMPONENT_H
