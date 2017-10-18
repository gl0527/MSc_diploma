#include "SoldierAnimComponent.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "InputHandler.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"


// ========================= class SoldierAnimComponent =========================

SoldierAnimComponent::SoldierAnimComponent (const std::string& name)
	: Component (name),
	m_entity (nullptr)
{
}


SoldierAnimComponent::~SoldierAnimComponent ()
{
}


void SoldierAnimComponent::Start ()
{
	if (auto& ownerMesh = m_owner->getFirstComponentByType<MeshComponent> ().lock ()) {
		m_entity = ownerMesh->getEntity ();

		if (auto skeleton = m_entity->getSkeleton ()) {
			skeleton->setBlendMode (Ogre::ANIMBLEND_CUMULATIVE);
			SetCurrentState (new IdleState (this));

			return;
		}
	}
	m_owner->removeComponent (m_name);
}


void SoldierAnimComponent::PostUpdate (float t, float dt)
{
	MakeStep (t, dt);
}


// =============================== class IdleState ===============================

SoldierAnimComponent::IdleState::IdleState (SoldierAnimComponent* parent)
	: AnimState ("Idle"),
	m_pParent (parent)
{
	if (auto animState = m_pParent->m_entity->getAnimationState ("leg_stand"))
		AddAnimState (animState);

	if (auto animState = m_pParent->m_entity->getAnimationState ("up_weapon_hold"))
		AddAnimState (animState);
}


void SoldierAnimComponent::IdleState::Execute (Stateable* stateable, float t, float dt)
{
	StepAll (dt);

	const InputHandler& inputHandler = InputHandler::GetInstance ();

	if (inputHandler.IsButtonDown (OIS::KC_LSHIFT) || inputHandler.IsButtonDown (OIS::KC_RSHIFT))	// kulon komponnesbe kellene tenni
		stateable->SetCurrentState (new RunState (m_pParent));

	if (inputHandler.IsLeftMouseButtonDown ())
		stateable->SetCurrentState (new ShootState (m_pParent));
}


// ================================ class RunState ================================

SoldierAnimComponent::RunState::RunState (SoldierAnimComponent* parent)
	: AnimState ("Run"),
	m_pParent (parent)
{
	if (auto animState = m_pParent->m_entity->getAnimationState ("leg_run"))
		AddAnimState (animState);

	if (auto animState = m_pParent->m_entity->getAnimationState ("up_run"))
		AddAnimState (animState);
}


void SoldierAnimComponent::RunState::Enter (Stateable* stateable)
{
	for (const auto& elem : m_animMap) {
		elem.second->setWeight (1.0f);
		elem.second->setLoop (false);
		elem.second->setTimePosition (0.0f);
		elem.second->setEnabled (true);
	}
}


void SoldierAnimComponent::RunState::Execute (Stateable* stateable, float t, float dt)
{
	StepAll (dt);

	const InputHandler& inputHandler = InputHandler::GetInstance ();

	if (!inputHandler.IsButtonDown (OIS::KC_LSHIFT) &&
		!inputHandler.IsButtonDown (OIS::KC_RSHIFT)) {
		stateable->SetCurrentState (new IdleState (m_pParent));
		return;
	}
	else if (HasEnded ("leg_run")) {
		for (const auto& elem : m_animMap)
			elem.second->setTimePosition (0.0f);
	}

	const Ogre::Vector3& forward = -1.0f * m_pParent->m_owner->Transform()->forward ();	// a -1-es szorzo csak azert kell, mert a katona alap nezeti iranya a z=+1, s nem z=-1

 	auto ownerPhysics = m_pParent->m_owner->getFirstComponentByType<PhysicsComponent>();

	if (auto phy = ownerPhysics.lock ()) {
		phy->SetLinearVelocity (0.0f, 0.0f, 0.0f);
		phy->ActivateRigidBody ();
		phy->AddForce (4'000.0f * forward.x, 4'000.0f * forward.y, 4'000.0f * forward.z);
	}

	if (inputHandler.IsLeftMouseButtonDown ())
		stateable->SetCurrentState (new ShootState (m_pParent));
}


// =============================== class ShootState ===============================

SoldierAnimComponent::ShootState::ShootState (SoldierAnimComponent* parent)
	: AnimState ("Shoot"),
	m_pParent (parent)
{
	if (auto animState = m_pParent->m_entity->getAnimationState ("up_shoot"))
		AddAnimState (animState);
}


void SoldierAnimComponent::ShootState::Enter (Stateable* stateable)
{
	for (const auto& elem : m_animMap) {
		elem.second->setWeight (1.0f);
		elem.second->setLoop (false);
		elem.second->setTimePosition (0.0f);
		elem.second->setEnabled (true);
	}
}


void SoldierAnimComponent::ShootState::Execute (Stateable* stateable, float t, float dt)
{
	Step ("up_shoot", dt);

	if (HasEnded ("up_shoot"))
		stateable->SetCurrentState (new IdleState (m_pParent));
}

