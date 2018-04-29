#include "RenderComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "RenderSystem.h"
#include "TransformComponent.h"
#include "OgreSceneManager.h"
#include "OgreEntity.h"


namespace Engine {

RenderComponent::RenderComponent (const std::string& name)
	: Component (name),
	m_pParentNode (Game::GetInstance ().GetRenderSystem ()->GetRootNode ()),
	m_pCurrentNode (nullptr),
	m_pObject (nullptr),
	m_pSceneManager (Game::GetInstance ().GetRenderSystem ()->GetSceneManager ())
{
}


RenderComponent::~RenderComponent ()
{
}


void RenderComponent::PostInit (GameObject* obj)
{
	CreateNode ();
}


void RenderComponent::PostUpdate (float t, float dt)
{
	m_pCurrentNode->setPosition (m_owner->Transform ()->GetLocalPosition ());
	m_pCurrentNode->setOrientation (m_owner->Transform ()->GetLocalRotation ());
	m_pCurrentNode->setScale (m_owner->Transform ()->GetLocalScale ());
}


void RenderComponent::Destroy ()
{
	m_pSceneManager->destroySceneNode (m_pCurrentNode);
	m_pCurrentNode = nullptr;
}


void RenderComponent::CreateNode ()
{
	if (const auto& ownerParent = m_owner->GetParent ().lock ()) {
		if (const auto& ownerRenderer = ownerParent->GetFirstComponentByType<RenderComponent> ().lock ()) {
			if (auto pNode = ownerRenderer->GetOgreNode ())
				m_pParentNode = pNode;
		}
	}
	m_pCurrentNode = m_pParentNode->createChildSceneNode ();
	m_pCurrentNode->attachObject (m_pObject);
}


void RenderComponent::MoveNode ()
{
	if (m_pCurrentNode != nullptr)
		m_pParentNode->removeChild (m_pCurrentNode);

	if (const auto& ownerParent = m_owner->GetParent ().lock ()) {
		if (const auto& ownerRenderer = ownerParent->GetFirstComponentByType<RenderComponent> ().lock ()) {
			if (auto pNode = ownerRenderer->GetOgreNode ())
				m_pParentNode = pNode;
		}
	}
	m_pParentNode->addChild (m_pCurrentNode);
}


void RenderComponent::SetCastShadows (bool cast)
{
	if (m_pObject)
		m_pObject->setCastShadows (cast);
}


void RenderComponent::SetVisible (bool visible)
{
	if (m_pObject)
		m_pObject->setVisible (visible);
}

}	// namespace Engine
