#include "DynamicMovementComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "TransformComponent.h"


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
	ownerPhysics = m_owner->getFirstComponentByType<PhysicsComponent>();
	ownerSoldierComp = m_owner->getFirstComponentByType<SoldierAnimComponent>();
}


void DynamicMovementComponent::PreUpdate(float t, float dt)
{
	Ogre::Vector3 force = Ogre::Vector3::ZERO;
	InputHandler& inputHandler = InputHandler::GetInstance ();

	if (inputHandler.IsButtonDown(OIS::KC_ESCAPE))
	{
		Game::GetInstance().Destroy();
		return;
	}
	if (inputHandler.IsButtonDown(OIS::KC_D))
	{
		force += Ogre::Vector3(1.0f, 0.0f, 0.0f);
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_RUN);
	}
	if (inputHandler.IsButtonDown(OIS::KC_A))
	{
		force += Ogre::Vector3(-1.0f, 0.0f, 0.0f);
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_RUN);
	}
	if (inputHandler.IsButtonDown(OIS::KC_W))
	{
		force += Ogre::Vector3(0.0f, 0.0f, -1.0f);
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_RUN);
	}
	if (inputHandler.IsButtonDown(OIS::KC_S))
	{
		force += Ogre::Vector3(0.0f, 0.0f, 1.0f);
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_RUN);
	}

	if (inputHandler.IsLeftMouseButtonDown ())
		if (auto soldierComp = ownerSoldierComp.lock());
			//soldierComp->setAction(SoldierComponent::PlayerAction::PA_SHOOT);

	/*Ogre::Radian yaw(-0.003 * ms.X.rel);
	Ogre::Quaternion Qyaw(yaw, Ogre::Vector3::UNIT_Y);
	ownerObject->transform()->setRotation(Qyaw * ownerObject->transform()->rotation());

	const Ogre::Vector3& right = ownerObject->transform()->right();
	Ogre::Radian pitch(-0.003 * ms.Y.rel);
	Ogre::Quaternion Qpitch(pitch, right);
	ownerObject->transform()->setRotation(Qpitch * ownerObject->transform()->rotation());*/

	force.normalise();
	force = m_owner->Transform()->worldRotation() * force; // azert, hogy a movedir az ownerObject koordinata-rendszereben legyen ertve

	if (auto phy = ownerPhysics.lock())
	{
		phy->SetLinearVelocity(0.0f, 0.0f, 0.0f);
		phy->ActivateRigidBody();
		phy->AddForce(moveSpeed * force.x, moveSpeed * force.y, moveSpeed * force.z);
	}
}
