#include "RenderComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "RenderSystem.h"
#include "TransformComponent.h"


namespace Engine {

RenderComponent::RenderComponent (const std::string& name)
	: Component (name),
	m_pParentNode (Game::GetInstance ().GetRenderSystem ()->getRootNode ()),
	m_pCurrentNode (nullptr),
	m_pObject (nullptr),
	m_pSceneManager (Game::GetInstance ().GetRenderSystem ()->getSceneManager ())
{
}


void RenderComponent::PostInit (GameObject* obj)
{
	CreateNode ();
	m_pCurrentNode->attachObject (m_pObject);
}


void RenderComponent::PostUpdate (float t, float dt)
{
	m_pCurrentNode->setPosition (m_owner->Transform ()->worldPosition ());
	m_pCurrentNode->setOrientation (m_owner->Transform ()->worldRotation ());
	m_pCurrentNode->setScale (m_owner->Transform ()->worldScale ());
}


void RenderComponent::Destroy ()
{
	m_pSceneManager->destroySceneNode (m_pCurrentNode);
	m_pCurrentNode = nullptr;
}


void RenderComponent::CreateNode ()
{
	if (const auto& ownerParent = m_owner->GetParent ().lock ()) {
		if (const auto& ownerRenderer = ownerParent->getFirstComponentByType<RenderComponent> ().lock ()) {
			if (auto pNode = ownerRenderer->GetOgreNode ())
				m_pParentNode = pNode;
		}
	}
	m_pCurrentNode = m_pParentNode->createChildSceneNode ();
}


RenderComponent::~RenderComponent ()
{
	std::cout << "RenderComponent destructor called.\n";
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
