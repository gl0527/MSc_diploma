#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"
#include "OgreRay.h"
#include "OgreVector3.h"


namespace Ogre {
class Camera;
class Viewport;
class RenderTexture;
class RenderWindow;
}


namespace Engine {

// =========================== class CameraComponent ============================

class CameraComponent : public RenderComponent
{
public:
	struct Descriptor
	{
		Descriptor ();

		std::string name;
		int zOrder;

		float nearClip;
		float farClip;
		Ogre::Vector3 lookat;
		float renderDist;
	};

					CameraComponent (const std::string& name, int zDepth);
					CameraComponent (const Descriptor& desc);
	virtual			~CameraComponent ();

	virtual void	PostInit (GameObject* obj) override;
	virtual void	Destroy () override;

	void			ApplyDescriptor (const Descriptor& desc);

	Ogre::Camera*	GetCamera () const;
	Ogre::Viewport* GetViewPort () const;
	Ogre::Ray		GetRay (float screenX, float screenY) const;

	void			SetLookAt (const Ogre::Vector3& newLookAt);
	void			SetNearClip (float nclip);
	void			SetFarClip (float fclip);
	void			SetRenderDist (float dist);
	void			SetRenderTexture (Ogre::RenderTexture* rt);

protected:
	int						m_zOrder;
	Ogre::Camera*			m_pCamera;
	Ogre::Viewport*			m_pViewport;
	Ogre::RenderWindow*		m_pRenderWnd;
	Ogre::RenderTexture*	m_pRenderTex;
};

}	// namespace Engine

#endif	// #ifndef CAMERA_COMPONENT_H
