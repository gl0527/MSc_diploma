#include "EnemyAIComponent.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "PlayerDataComponent.h"


EnemyAIComponent::EnemyAIComponent (const std::string& name):
	Component (name),
	m_enemyStateMachine (State::Search),
	m_targetObj (nullptr)
{
	m_enemyStateMachine.AddTransitions ({
		{State::Search, State::Attack, 'a', nullptr},
		{State::Search, State::RunAway, 'r', nullptr},
		{State::Attack, State::Search, 's', nullptr},
		{State::Attack, State::RunAway, 'r', nullptr},
	});

	m_enemyStateMachine.AddStateFunction (State::Search, 
		std::bind (&EnemyAIComponent::OnSearch, this, std::placeholders::_1, std::placeholders::_2));

	m_enemyStateMachine.AddStateFunction (State::Attack,
		std::bind (&EnemyAIComponent::OnAttack, this, std::placeholders::_1, std::placeholders::_2));

	m_enemyStateMachine.AddStateFunction (State::RunAway,
		std::bind (&EnemyAIComponent::OnRunAway, this, std::placeholders::_1, std::placeholders::_2));
}


void EnemyAIComponent::Start ()
{
	if (auto obj = ObjectManager::GetInstance ().GetGameObjectByName ("gijoe").lock ())
		m_targetObj = obj;
	if (m_targetObj == nullptr) {
		m_owner->RemoveComponent (m_name);
		return;
	}

	if (auto phy = m_owner->GetFirstComponentByType<PhysicsComponent> ().lock ())
		m_pOwnerPhysics = phy;
	if (m_pOwnerPhysics == nullptr) {
		m_owner->RemoveComponent (m_name);
		return;
	}

	m_pOwnerPhysics->onCollision += std::bind (&EnemyAIComponent::OnCollisionWithPlayer, this, std::placeholders::_1);

	if (auto ownerData = m_owner->GetFirstComponentByType<PlayerDataComponent> ().lock ())
		m_ownerData = ownerData;

	if (m_ownerData == nullptr) {
		m_owner->RemoveComponent (m_name);
		return;
	}
}


void EnemyAIComponent::PreUpdate (float t, float dt)
{
	const Ogre::Vector3& targetWorldPos = GetTargetPosition ();
	const Ogre::Vector3& ownerWorldPos = GetOwnerPosition ();
	
	if (m_ownerData->GetHealthPoint () < 1) {
		m_enemyStateMachine.SetState (State::Dead);
	} else if (m_ownerData->GetHealthPoint () < 2) {
		m_enemyStateMachine.Process ('r');
	} else if (targetWorldPos.distance (ownerWorldPos) < 30.0f) {
		m_enemyStateMachine.Process ('a');
	} else {
		m_enemyStateMachine.Process ('s');
	}

	m_enemyStateMachine.Update (t, dt);
}


const Ogre::Vector3& EnemyAIComponent::GetTargetPosition () const
{
	return m_targetObj->Transform ()->GetGlobalPosition ();
}


const Ogre::Vector3& EnemyAIComponent::GetOwnerPosition () const
{
	return m_owner->Transform ()->GetGlobalPosition ();
}


Ogre::Vector3 EnemyAIComponent::GetOwnerFacing () const
{
	return -m_owner->Transform ()->GetGlobalFacing ();	// enemies modelled with +z direction
}


void EnemyAIComponent::Move (float distance, bool follow)
{
	const Ogre::Vector3& targetWorldPos = GetTargetPosition ();
	const Ogre::Vector3& ownerWorldPos = GetOwnerPosition ();
	Ogre::Vector3 ownerWorldFacing = GetOwnerFacing ();

	if (targetWorldPos.distance (ownerWorldPos) > distance) {
		Ogre::Vector3 directionIn3D = follow ? targetWorldPos - ownerWorldPos : ownerWorldPos - targetWorldPos;
		Ogre::Vector3 direction = Ogre::Vector3 (directionIn3D.x, targetWorldPos.y, directionIn3D.z);
		
		direction.normalise ();
		ownerWorldFacing.normalise ();

		Ogre::Real turnAngle = Ogre::Math::ACos (ownerWorldFacing.dotProduct (direction)).valueRadians ();
		char sign = 1;
		if (turnAngle > Ogre::Math::PI) {
			turnAngle = 2 * Ogre::Math::PI - turnAngle;
			sign = -1;
		}

		m_pOwnerPhysics->ActivateRigidBody ();
		m_pOwnerPhysics->SetLinearVelocity (0.0f, 0.0f, 0.0f);
		m_pOwnerPhysics->SetAngularVelocity (0.0f, 0.0f, 0.0f);
		m_pOwnerPhysics->AddTorque (0.0f, 300 * sign * turnAngle, 0.0f);
		m_pOwnerPhysics->AddForce (1000 * direction.x, 1000 * direction.y, 1000 * direction.z);
	}
}


void EnemyAIComponent::OnSearch (float /*t*/, float /*dt*/)
{
	Move (10.0f, true);
}


void EnemyAIComponent::OnAttack (float /*t*/, float /*dt*/)
{
	Move (4.0f, true);
}


void EnemyAIComponent::OnRunAway (float /*t*/, float /*dt*/)
{
	Move (10.0f, false);
}


void EnemyAIComponent::OnCollisionWithPlayer (PhysicsComponent* other)
{
	static unsigned int counter = 0;

	if (counter % 30 == 0) {
		GameObject* otherOwner = other->GetOwner ();

		if (!otherOwner->HasTag ("player"))
			return;

		if (auto otherPlayerData = otherOwner->GetFirstComponentByType<PlayerDataComponent> ().lock ())
			otherPlayerData->DecreaseHealtPoint (1);
	}

	++counter;
}


EnemyAIComponent::State EnemyAIComponent::GetState () const
{
	return m_enemyStateMachine.GetState ();
}
