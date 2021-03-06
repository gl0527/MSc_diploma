#include "DynamicMovementComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "InputManager.h"
#include "PhysicsComponent.h"
#include "PlayerDataComponent.h"
#include "WeaponComponent.h"
#include "AudioManager.h"
#include "PlayerDataComponent.h"


DynamicMovementComponent::DynamicMovementComponent (const std::string& name)
	:Component (name),
	m_moveSpeed (0.0f),
	m_turnSpeed (0.0f),
	m_pOwnerPhysics (nullptr),
	m_pOwnerData (nullptr)
{
}


void DynamicMovementComponent::Start ()
{
	if (auto ownerData = m_owner->GetFirstComponentByType<PlayerDataComponent> ().lock ())
		m_pOwnerData = ownerData;

	if (m_pOwnerData == nullptr) {
		m_owner->RemoveComponent (m_name);
		return;
	}

	if (auto ownerPhysics = m_owner->GetFirstComponentByType<PhysicsComponent> ().lock ())
		m_pOwnerPhysics = ownerPhysics;

	if (m_pOwnerPhysics == nullptr) {
		m_owner->RemoveComponent (m_name);
		return;
	}

	m_pOwnerPhysics->onCollision += std::bind (&DynamicMovementComponent::OnCollisionWithWeapon, this, std::placeholders::_1);
	m_pOwnerPhysics->onCollision += std::bind (&DynamicMovementComponent::OnCollisionWithTable, this, std::placeholders::_1);
}


void DynamicMovementComponent::PreUpdate (float t, float dt)
{
	if (m_pOwnerData->IsDead ()) {
		m_owner->RemoveComponent (m_name);
		return;
	}
	
	Ogre::Vector3 dForce = Ogre::Vector3::ZERO;

	const InputManager& inputManager = InputManager::GetInstance ();

	if (inputManager.IsKeyDown (OIS::KC_ESCAPE)) {
		AudioManager::GetInstance ().Disable ();
		Game::GetInstance ().Pause ();
		return;
	}
	if (inputManager.IsKeyDown (OIS::KC_W))
		dForce += Ogre::Vector3 (0.0f, 0.0f, 1.0f);
	if (inputManager.IsKeyDown (OIS::KC_S))
		dForce += Ogre::Vector3 (0.0f, 0.0f, -1.0f);

	int mouseXRel;
	if (!inputManager.GetRelativeMouseX (&mouseXRel))
		return;

	dForce.normalise ();
	dForce = m_owner->Transform ()->GetGlobalRotation () * dForce;

	m_pOwnerPhysics->ActivateRigidBody ();
	m_pOwnerPhysics->SetLinearVelocity (0.0f, 0.0f, 0.0f);
	m_pOwnerPhysics->SetAngularVelocity (0.0f, 0.0f, 0.0f);
	m_pOwnerPhysics->AddForce (m_moveSpeed * dForce.x, m_moveSpeed * dForce.y, m_moveSpeed * dForce.z);
	m_pOwnerPhysics->AddTorque (0.0f, -m_turnSpeed * mouseXRel, 0.0f);
}


void DynamicMovementComponent::SetMoveSpeed (float moveSpeed)
{
	m_moveSpeed = moveSpeed;
}


void DynamicMovementComponent::SetTurnSpeed (float turnSpeed)
{
	m_turnSpeed = turnSpeed;
}


void DynamicMovementComponent::OnCollisionWithWeapon (PhysicsComponent* other)
{
	static bool collided = false;
	
	GameObject* otherOwner = other->GetOwner ();
	if (!collided && otherOwner->GetName () == "weapon") {
		collided = true;
		other->SetTypeToKinematic ();
		otherOwner->SetParent (m_owner->GetName ());
		if (auto ownerData = m_owner->GetFirstComponentByType<PlayerDataComponent> ().lock ())
			ownerData->SetWeapon (true);
	}
}


void DynamicMovementComponent::OnCollisionWithTable (PhysicsComponent* other)
{
	GameObject* otherOwner = other->GetOwner ();

	if (otherOwner->HasTag ("ammo")) {
		if (auto weapon = m_owner->GetChild ("weapon")) {
			if (auto weaponComp = weapon->GetFirstComponentByType<WeaponComponent> ().lock ())
				weaponComp->SetAmmoToFull ();
		}
	}
}
