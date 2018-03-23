#include "DynamicMovementComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "TransformComponent.h"
#include "InputManager.h"


DynamicMovementComponent::DynamicMovementComponent(const std::string& name)
	:Component(name),
	moveSpeed(0.0f),
	ownerPhysics(std::shared_ptr<PhysicsComponent>(nullptr)),
	ownerSoldierComp(std::shared_ptr<SoldierAnimComponent>(nullptr))
{
}


DynamicMovementComponent::~DynamicMovementComponent()
{
}


void DynamicMovementComponent::Start()
{
	ownerPhysics = m_owner->GetFirstComponentByType<PhysicsComponent>();
	ownerSoldierComp = m_owner->GetFirstComponentByType<SoldierAnimComponent>();
}


void DynamicMovementComponent::PreUpdate(float t, float dt)
{
	Ogre::Vector3 force = Ogre::Vector3::ZERO;
	Ogre::Vector3 torque = Ogre::Vector3::ZERO;

	const InputManager& inputManager = InputManager::GetInstance ();

	if (inputManager.IsKeyDown(OIS::KC_ESCAPE))
	{
		Game::GetInstance().Destroy();
		return;
	}
	if (inputManager.IsKeyDown(OIS::KC_D))
	{
		force += Ogre::Vector3(-1.0f, 0.0f, 0.0f);
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_RUN);
	}
	if (inputManager.IsKeyDown(OIS::KC_A))
	{
		force += Ogre::Vector3(1.0f, 0.0f, 0.0f);
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_RUN);
	}
	if (inputManager.IsKeyDown(OIS::KC_W))
	{
		force += Ogre::Vector3(0.0f, 0.0f, 1.0f);
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_RUN);
	}
	if (inputManager.IsKeyDown(OIS::KC_S))
	{
		force += Ogre::Vector3(0.0f, 0.0f, -1.0f);
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_RUN);
	}
	if (inputManager.IsKeyDown (OIS::KC_Q)) {
		torque += Ogre::Vector3 (0.0f, 1.0f, 0.0f);
	}
	if (inputManager.IsKeyDown (OIS::KC_E)) {
		torque += Ogre::Vector3 (0.0f, -1.0f, 0.0f);
	}

	if (inputManager.IsLeftMouseButtonDown ())
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_SHOOT);

	/*int mouseXRel, mouseYRel;

	if (inputManager.GetRelativeMouseX (&mouseXRel)) {
		torque += Ogre::Vector3 (mouseXRel, 0.0f, 0.0f);
	}*/

	/*Ogre::Radian yaw(-0.003 * ms.X.rel);
	Ogre::Quaternion Qyaw(yaw, Ogre::Vector3::UNIT_Y);
	ownerObject->transform()->setRotation(Qyaw * ownerObject->transform()->rotation());

	const Ogre::Vector3& right = ownerObject->transform()->right();
	Ogre::Radian pitch(-0.003 * ms.Y.rel);
	Ogre::Quaternion Qpitch(pitch, right);
	ownerObject->transform()->setRotation(Qpitch * ownerObject->transform()->rotation());*/

	force.normalise();
	torque.normalise ();

	force = m_owner->Transform()->GetGlobalRotation() * force; // azert, hogy a movedir az ownerObject koordinata-rendszereben legyen ertve
	torque = m_owner->Transform ()->GetGlobalRotation () * torque;

	if (auto phy = ownerPhysics.lock())
	{
		phy->SetLinearVelocity(0.0f, 0.0f, 0.0f);
		phy->ActivateRigidBody();
		phy->AddForce(moveSpeed * force.x, moveSpeed * force.y, moveSpeed * force.z);
		phy->AddTorque (100.0f * torque.x, 100.0f * torque.y, 100.0f * torque.z);
	}
}
