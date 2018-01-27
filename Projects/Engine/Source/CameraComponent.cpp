#include "CameraComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "RenderSystem.h"


namespace Engine {

CameraComponent::Descriptor::Descriptor ()
	: name (""),
	zOrder (0),
	nearClip (0.0f),
	farClip (0.0f),
	lookat (Ogre::Vector3::ZERO),
	renderDist (0.0f)
{
}


CameraComponent::CameraComponent (const std::string& name, int zDepth)
	: RenderComponent (name),
	m_zOrder (zDepth),
	m_pCamera (nullptr),
	m_pViewport (nullptr),
	m_pRenderWnd (Game::GetInstance ().GetRenderSystem ()->GetRenderWindow ()),
	m_pRenderTex (nullptr)
{
}


CameraComponent::CameraComponent (const Descriptor& desc)
	: CameraComponent (desc.name, desc.zOrder)
{
}


CameraComponent::~CameraComponent ()
{
}


void CameraComponent::PostInit (GameObject* obj)
{
	while (m_pRenderWnd->hasViewportWithZOrder (m_zOrder))
		++m_zOrder;

	m_pCamera = m_pSceneManager->createCamera (obj->GetName () + Ogre::StringConverter::toString (m_zOrder));

	m_pViewport = m_pRenderWnd->addViewport (m_pCamera, m_zOrder);
	m_pCamera->setAspectRatio (Ogre::Real (m_pViewport->getActualWidth ()) / Ogre::Real (m_pViewport->getActualHeight ()));
	m_pObject = m_pCamera;

	CreateNode ();
	m_pCurrentNode->attachObject (m_pObject);
}


void CameraComponent::Destroy ()
{
	if (m_pRenderWnd)
		m_pRenderWnd->removeViewport (m_zOrder);
	if (m_pSceneManager) {
		m_pSceneManager->destroyCamera (m_pCamera->getName ());
		m_pSceneManager->destroySceneNode (m_pCurrentNode);
	}
	m_pCurrentNode = nullptr;
	//sceneMgr->destroyEntity(camera->getName());
}


void CameraComponent::ApplyDescriptor (const Descriptor& desc)
{
	SetLookAt (desc.lookat);
	SetNearClip (desc.nearClip);
	SetFarClip (desc.farClip);
	SetRenderDist (desc.renderDist);
}


Ogre::Camera* CameraComponent::GetCamera () const
{
	return m_pCamera;
}


Ogre::Viewport* CameraComponent::GetViewPort () const
{
	return m_pViewport;
}


Ogre::Ray CameraComponent::GetRay (float screenX, float screenY) const
{
	return m_pCamera->getCameraToViewportRay (screenX, screenY);
}


void CameraComponent::SetLookAt (const Ogre::Vector3& newLookAt)
{
	m_pCamera->lookAt (newLookAt);
}


void CameraComponent::SetNearClip (float nclip)
{
	m_pCamera->setNearClipDistance (nclip);
}


void CameraComponent::SetFarClip (float fclip)
{
	m_pCamera->setFarClipDistance (fclip);
}


void CameraComponent::SetRenderDist (float dist)
{
	m_pCamera->setRenderingDistance (dist);
}


void CameraComponent::SetRenderTexture (Ogre::RenderTexture* rt)
{
	m_pRenderTex = rt;
	m_pViewport = m_pRenderTex->addViewport (m_pCamera, m_zOrder + 1);
	//renderWnd->removeViewport(zOrder);

}

}	// namespace Engine
