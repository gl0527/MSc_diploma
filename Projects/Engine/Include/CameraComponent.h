#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"
#include "Ogre.h"


namespace Engine {

// =========================== class CameraComponent ============================

class CameraComponent : public RenderComponent
{
public:
	struct Descriptor
	{
		Descriptor ()
			: name (""),
			zOrder (0),
			nearClip (0.0f),
			farClip (0.0f),
			lookat (Ogre::Vector3::ZERO),
			renderDist (0.0f)
		{}

		std::string name;
		int zOrder;

		float nearClip;
		float farClip;
		Ogre::Vector3 lookat;
		float renderDist;
	};

	CameraComponent (const std::string& name, int zDepth);
	CameraComponent (const Descriptor& desc);
	virtual ~CameraComponent ();

	virtual void PostInit (GameObject* obj) override;
	virtual void Destroy () override;

	void ApplyDescriptor (const Descriptor& desc);

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
