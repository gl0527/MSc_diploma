#include "CameraComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "RenderSystem.h"
#include "Ogre.h"


namespace Engine
{

CameraComponent::CameraComponent (const std::string& name, int zDepth)
	: RenderComponent (name),
	zOrder (zDepth),
	camera (nullptr),
	viewport (nullptr),
	renderWnd (Game::GetInstance ().GetRenderSystem ()->getRenderWindow ()),
	renderTexture (nullptr)
{
}


CameraComponent::~CameraComponent ()
{
}


void CameraComponent::Init (GameObject* obj)
{
	m_owner = obj;
	camera = m_pSceneManager->createCamera (obj->GetName () + Ogre::StringConverter::toString (zOrder));

	while (renderWnd->hasViewportWithZOrder (zOrder))
		++zOrder;

	viewport = renderWnd->addViewport (camera, zOrder);
	camera->setAspectRatio (Ogre::Real (viewport->getActualWidth ()) / Ogre::Real (viewport->getActualHeight ()));
	m_pObject = camera;

	CreateNode ();
	m_pCurrentNode->attachObject (m_pObject);
}


void CameraComponent::Destroy ()
{
	if (renderWnd)
		renderWnd->removeViewport (zOrder);
	if (m_pSceneManager) {
		m_pSceneManager->destroyCamera (camera->getName ());
		m_pSceneManager->destroySceneNode (m_pCurrentNode);
	}
	m_pCurrentNode = nullptr;
	//sceneMgr->destroyEntity(camera->getName());
}


Ogre::Camera* CameraComponent::getCamera () const
{ 
	return camera; 
}


Ogre::Viewport* CameraComponent::getViewPort () const 
{
	return viewport; 
}


Ogre::Ray CameraComponent::getRay (float screenX, float screenY) const
{
	return camera->getCameraToViewportRay (screenX, screenY);
}


void CameraComponent::setLookAt (const Ogre::Vector3& newLookAt)
{
	camera->lookAt (newLookAt);
}


void CameraComponent::setNearClip (float nclip)
{
	camera->setNearClipDistance (nclip);
}


void CameraComponent::setFarClip (float fclip)
{
	camera->setFarClipDistance (fclip);
}


void CameraComponent::setRenderDist (float dist)
{
	camera->setRenderingDistance (dist);
}


void CameraComponent::setRenderTexture (Ogre::RenderTexture* rt)
{
	renderTexture = rt;
	viewport = renderTexture->addViewport (camera, zOrder + 1);
	//renderWnd->removeViewport(zOrder);

}

}	// namespace Engine
