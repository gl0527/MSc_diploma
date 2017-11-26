#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"


// ----------------------------- forward declaration -----------------------------

namespace Ogre {
class Camera;
class Viewport;
class Ray;
class RenderWindow;
class RenderTexture;
class Vector3;
}

namespace Engine {

// =========================== class CameraComponent ============================

class CameraComponent : public RenderComponent
{
public:
	CameraComponent (const std::string& name, int zDepth);
	virtual ~CameraComponent ();

	virtual void PostInit (GameObject* obj) override;
	virtual void Destroy () override;

	Ogre::Camera* getCamera () const;
	Ogre::Viewport* getViewPort () const;
	Ogre::Ray getRay (float screenX, float screenY) const;

	void setLookAt (const Ogre::Vector3& newLookAt);
	void setNearClip (float nclip);
	void setFarClip (float fclip);
	void setRenderDist (float dist);
	void setRenderTexture (Ogre::RenderTexture* rt);

protected:
	int zOrder;
	Ogre::Camera* camera;
	Ogre::Viewport* viewport;
	Ogre::RenderWindow* renderWnd;
	Ogre::RenderTexture* renderTexture;
};

}	// namespace Engine

#endif	// #ifndef CAMERA_COMPONENT_H
