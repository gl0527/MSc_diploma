#include "DynamicMovementComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "TransformComponent.h"
#include "InputManager.h"


DynamicMovementComponent::DynamicMovementComponent (const std::string& name)
	:Component (name),
	moveSpeed (0.0f),
	turnSpeed (0.0f),
	ownerPhysics (std::shared_ptr<PhysicsComponent> (nullptr)),
	ownerSoldierComp (std::shared_ptr<SoldierAnimComponent> (nullptr))
{
}


DynamicMovementComponent::~DynamicMovementComponent ()
{
}


void DynamicMovementComponent::Start ()
{
	ownerPhysics = m_owner->GetFirstComponentByType<PhysicsComponent> ();
	ownerSoldierComp = m_owner->GetFirstComponentByType<SoldierAnimComponent> ();
}


void DynamicMovementComponent::PreUpdate (float t, float dt)
{
	Ogre::Vector3 dForce = Ogre::Vector3::ZERO;
	float dTorque = 0.0f;

	const InputManager& inputManager = InputManager::GetInstance ();

	if (inputManager.IsKeyDown (OIS::KC_ESCAPE)) {
		Game::GetInstance ().Destroy ();
		return;
	}
	if (inputManager.IsKeyDown (OIS::KC_W))
		dForce += Ogre::Vector3 (0.0f, 0.0f, 1.0f);
	if (inputManager.IsKeyDown (OIS::KC_S))
		dForce += Ogre::Vector3 (0.0f, 0.0f, -1.0f);

	int mouseXRel;
	if (inputManager.GetRelativeMouseX (&mouseXRel))
		dTorque -= mouseXRel;

	dForce.normalise ();
	dForce = m_owner->Transform ()->GetGlobalRotation () * dForce;

	if (auto phy = ownerPhysics.lock ()) {
		phy->ActivateRigidBody ();
		phy->SetLinearVelocity (0.0f, 0.0f, 0.0f);
		phy->SetAngularVelocity (0.0f, 0.0f, 0.0f);
		phy->AddForce (moveSpeed * dForce.x, moveSpeed * dForce.y, moveSpeed * dForce.z);
		phy->AddTorque (0.0f, turnSpeed * dTorque, 0.0f);
	}
}
