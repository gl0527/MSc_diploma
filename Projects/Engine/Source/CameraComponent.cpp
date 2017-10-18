#include "CameraComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "RenderSystem.h"


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


Ogre::Ray CameraComponent::getRay (float screenX, float screenY) const
{
	return camera->getCameraToViewportRay (screenX, screenY);
}


void CameraComponent::setRenderTexture (Ogre::RenderTexture* rt)
{
	renderTexture = rt;
	viewport = renderTexture->addViewport (camera, zOrder + 1);
	//renderWnd->removeViewport(zOrder);

}

}	// namespace Engine
