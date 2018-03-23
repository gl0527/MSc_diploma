#include "SoldierAnimComponent.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "InputManager.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "OgreVector3.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonInstance.h"
#include "OgreEntity.h"


// ========================= class SoldierAnimComponent =========================

SoldierAnimComponent::SoldierAnimComponent (const std::string& name)
	: Stateable (name),
	m_entity (nullptr)
{
}


SoldierAnimComponent::~SoldierAnimComponent ()
{
	/*if (IdleState::HasInstance ())
		IdleState::DeleteInstance ();

	if (RunState::HasInstance ())
		RunState::DeleteInstance ();

	if (ShootState::HasInstance ())
		ShootState::DeleteInstance ();*/
}


void SoldierAnimComponent::Start ()
{
	if (auto ownerMesh = m_owner->GetFirstComponentByType<MeshComponent> ().lock ()) {
		m_entity = ownerMesh->GetEntity ();

		if (auto skeleton = m_entity->getSkeleton ()) {
			skeleton->setBlendMode (Ogre::ANIMBLEND_CUMULATIVE);
			SetCurrentState (IdleState::GetInstance (this));

			return;
		}
	}
	m_owner->RemoveComponent (m_name);
}


// =============================== class IdleState ===============================

AnimState::SPtr SoldierAnimComponent::IdleState::s_pInstance = nullptr;


SoldierAnimComponent::IdleState::IdleState (SoldierAnimComponent* parent)
	: AnimState ("Idle"),
	m_pParent (parent)
{
	if (auto animState = m_pParent->m_entity->getAnimationState ("leg_stand")) {
		AddAnimState (animState);
	}

	if (auto animState = m_pParent->m_entity->getAnimationState ("up_weapon_hold")) {
		AddAnimState (animState);
	}
}


/*static*/ 
const AnimState::SPtr& SoldierAnimComponent::IdleState::GetInstance (SoldierAnimComponent* parent)
{
	if (s_pInstance == nullptr)
		s_pInstance.reset (new IdleState (parent));

	return s_pInstance;
}


/*static*/
void SoldierAnimComponent::IdleState::DeleteInstance ()
{
	if (s_pInstance)
		s_pInstance.reset ();
}


/*static*/
bool SoldierAnimComponent::IdleState::HasInstance ()
{
	return s_pInstance != nullptr;
}


void SoldierAnimComponent::IdleState::PostExecute (Stateable* stateable, float t, float dt)
{
	static bool switchToRunState = false;
	static bool switchToShootState = false;
	
	const InputManager& inputManager = InputManager::GetInstance ();

	if (!switchToShootState && (inputManager.IsKeyDown (OIS::KC_LSHIFT) || inputManager.IsKeyDown (OIS::KC_RSHIFT))) {
		switchToRunState = true;
	}

	if (switchToRunState) {
		const auto& nextState = RunState::GetInstance (m_pParent);

		static bool switchToRunStateFirstHit = false;
		if (!switchToRunStateFirstHit) {
			nextState->SetWeightAll (0.0f);
			nextState->EnableAll ();
			switchToRunStateFirstHit = true;
		}

		if (Blend (nextState)) {
			switchToRunStateFirstHit = false;
			switchToRunState = false;
			stateable->SetCurrentState (nextState);
		}

		return;
	}

	if (!switchToRunState) {
		bool b = inputManager.IsLeftMouseButtonDown ();
		if (b)
			switchToShootState = true;
	}

	if (switchToShootState) {
		const auto& nextState = ShootState::GetInstance (m_pParent);

		static bool firstHit = false;
		if (!firstHit) {
			nextState->SetWeightAll (0.0f);
			nextState->EnableAll ();
			firstHit = true;
		}

		if (Blend (nextState)) {
			firstHit = false;
			switchToShootState = false;
			stateable->SetCurrentState (nextState);
		}
	}
}


// ================================ class RunState ================================

AnimState::SPtr SoldierAnimComponent::RunState::s_pInstance = nullptr;


SoldierAnimComponent::RunState::RunState (SoldierAnimComponent* parent)
	: AnimState ("Run"),
	m_pParent (parent)
{
	if (auto animState = m_pParent->m_entity->getAnimationState ("leg_run")) {
		AddAnimState (animState);
	}

	if (auto animState = m_pParent->m_entity->getAnimationState ("up_run")) {
		AddAnimState (animState);
	}
}


/*static*/
const AnimState::SPtr& SoldierAnimComponent::RunState::GetInstance (SoldierAnimComponent* parent)
{
	if (s_pInstance == nullptr)
		s_pInstance.reset (new RunState (parent));

	return s_pInstance;
}


/*static*/
void SoldierAnimComponent::RunState::DeleteInstance ()
{
	if (s_pInstance)
		s_pInstance.reset ();
}


/*static*/
bool SoldierAnimComponent::RunState::HasInstance ()
{
	return s_pInstance != nullptr;
}


void SoldierAnimComponent::RunState::PostEnter (Stateable* stateable)
{
	for (const auto& elem : m_animMap) {
		elem.second->setWeight (1.0f);
		elem.second->setLoop (false);
	}
}


void SoldierAnimComponent::RunState::PostExecute (Stateable* stateable, float t, float dt)
{
	static bool toIdle = false;
	static bool toShoot = false;
	
	const InputManager& inputManager = InputManager::GetInstance ();

	if (!toShoot && !inputManager.IsKeyDown (OIS::KC_LSHIFT) && !inputManager.IsKeyDown (OIS::KC_RSHIFT)) {
		toIdle = true;
	} else if (HasEnded ("leg_run")) {
		toIdle = false;

		for (const auto& elem : m_animMap)
			elem.second->setTimePosition (0.0f);
	}

	if (toIdle) {
		const auto& nextState = IdleState::GetInstance (m_pParent);

		static bool toIdleFirstHit = false;
		if (!toIdleFirstHit) {
			nextState->SetWeightAll (0.0f);
			nextState->EnableAll ();
			toIdleFirstHit = true;
		}

		if (Blend (nextState, 0.0f, 0.02f)) {
			toIdleFirstHit = false;
			toIdle = false;
			stateable->SetCurrentState (nextState);
		}

		return;
	}

	const Ogre::Vector3& forward = -1.0f * m_pParent->m_owner->Transform()->Forward ();

 	auto ownerPhysics = m_pParent->m_owner->GetFirstComponentByType<PhysicsComponent>();

	if (auto phy = ownerPhysics.lock ()) {
		phy->SetLinearVelocity (0.0f, 0.0f, 0.0f);
		phy->ActivateRigidBody ();
		phy->AddForce (4'000.0f * forward.x, 4'000.0f * forward.y, 4'000.0f * forward.z);
		phy->SetAngularFactor (0.0f, 1.0f, 0.0f);
	}

	if (!toIdle && inputManager.IsLeftMouseButtonDown ())
		toShoot = true;

	if (toShoot) {
		const auto& nextState = ShootState::GetInstance (m_pParent);

		static bool firstHit = false;
		if (!firstHit) {
			nextState->SetWeightAll (0.0f);
			nextState->EnableAll ();
			firstHit = true;
		}

		if (Blend (nextState)) {
			firstHit = false;
			toShoot = false;
			stateable->SetCurrentState (nextState);
		}
	}
}


// =============================== class ShootState ===============================

AnimState::SPtr SoldierAnimComponent::ShootState::s_pInstance = nullptr;


SoldierAnimComponent::ShootState::ShootState (SoldierAnimComponent* parent)
	: AnimState ("Shoot"),
	m_pParent (parent)
{
	if (auto animState = m_pParent->m_entity->getAnimationState ("up_shoot")) {
		AddAnimState (animState);
	}
}


/*static*/
const AnimState::SPtr& SoldierAnimComponent::ShootState::GetInstance (SoldierAnimComponent* parent)
{
	if (s_pInstance == nullptr)
		s_pInstance.reset (new ShootState (parent));

	return s_pInstance;
}


/*static*/
void SoldierAnimComponent::ShootState::DeleteInstance ()
{
	if (s_pInstance)
		s_pInstance.reset ();
}


/*static*/
bool SoldierAnimComponent::ShootState::HasInstance ()
{
	return s_pInstance != nullptr;
}


void SoldierAnimComponent::ShootState::PostEnter (Stateable* stateable)
{
	for (const auto& elem : m_animMap) {
		elem.second->setWeight (1.0f);
		elem.second->setLoop (false);
	}
}


void SoldierAnimComponent::ShootState::PostExecute (Stateable* stateable, float t, float dt)
{
	const auto& nextState = IdleState::GetInstance (m_pParent);
	static bool firstHit = false;

	if (!firstHit) {
		nextState->SetWeightAll (0.0f);
		nextState->EnableAll ();
		firstHit = true;
	}
	if (Blend (nextState, 1.0f, 0.0f, 1.0f)) {
		firstHit = false;
		stateable->SetCurrentState (nextState);
	}
}

