#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"


namespace Engine
{

// =========================== class CameraComponent ============================

class DLL_SPEC CameraComponent : public RenderComponent
{
public:
	CameraComponent (const std::string& name, int zDepth);
	virtual ~CameraComponent ();

	virtual void Init (GameObject* obj) override;
	virtual void Destroy () override;

	Ogre::Camera* getCamera () const { return camera; }
	Ogre::Viewport* getViewPort () const { return viewport; }
	Ogre::Ray getRay (float screenX, float screenY) const;

	void setLookAt (const Ogre::Vector3& newLookAt) { camera->lookAt (newLookAt); }
	void setNearClip (float nclip) { camera->setNearClipDistance (nclip); }
	void setFarClip (float fclip) { camera->setFarClipDistance (fclip); }
	void setRenderDist (float dist) { camera->setRenderingDistance (dist); }
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
