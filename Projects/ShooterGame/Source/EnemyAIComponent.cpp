#include "EnemyAIComponent.h"
#include <iostream>
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
		{State::Search, State::Attack, 'a', [] { std::cout << "Time to attack you!\n"; }},
		{State::Search, State::RunAway, 'r', [] { std::cout << "Do not hurt me!\n"; }},
		{State::Attack, State::Search, 's', [] { std::cout << "Where are you?\n"; }},
		{State::Attack, State::RunAway, 'r', [] { std::cout << "Do not hurt me!\n"; }},
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

	if (auto ownerData = m_owner->GetFirstComponentByType<PlayerDataComponent> ().lock ())
		m_ownerData = ownerData;

	if (m_ownerData == nullptr) {
		m_owner->RemoveComponent (m_name);
		return;
	}
}


void EnemyAIComponent::PreUpdate (float t, float dt)
{
	Ogre::Vector3 targetWorldPos = m_targetObj->Transform ()->GetGlobalPosition ();
	Ogre::Vector3 ownerWorldPos = m_owner->Transform ()->GetGlobalPosition ();
	
	if (m_ownerData->GetHealthPoint () < 20)
		m_enemyStateMachine.Process ('r');
	else if (targetWorldPos.distance (ownerWorldPos) < 30.0f) {
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


const Ogre::Vector3& EnemyAIComponent::GetOwnerFacing () const
{
	return -m_owner->Transform ()->GetGlobalFacing ();	// enemies modelled with +z direction
}


void EnemyAIComponent::Move (float distance, bool follow)
{
	Ogre::Vector3 targetWorldPos = GetTargetPosition ();
	Ogre::Vector3 ownerWorldPos = GetOwnerPosition ();
	Ogre::Vector3 ownerWorldFacing = GetOwnerFacing ();

	if (targetWorldPos.distance (ownerWorldPos) > distance) {
		Ogre::Vector3 direction (follow ? targetWorldPos - ownerWorldPos : ownerWorldPos - targetWorldPos);
		direction.normalise ();
		ownerWorldFacing.normalise ();

		Ogre::Real cosTurnAngle (ownerWorldFacing.dotProduct (direction));
		Ogre::Radian turnAngle = Ogre::Math::ACos (cosTurnAngle);

		m_pOwnerPhysics->ActivateRigidBody ();
		m_pOwnerPhysics->SetLinearVelocity (0.0f, 0.0f, 0.0f);
		m_pOwnerPhysics->SetAngularVelocity (0.0f, 0.0f, 0.0f);
		m_pOwnerPhysics->AddTorque (0.0f, 300 * turnAngle.valueRadians (), 0.0f);
		m_pOwnerPhysics->AddForce (1000 * direction.x, 1000 * direction.y, 1000 * direction.z);
	}
}


void EnemyAIComponent::OnSearch (float /*t*/, float /*dt*/)
{
	Move (20.0f, true);
}


void EnemyAIComponent::OnAttack (float /*t*/, float /*dt*/)
{

}


void EnemyAIComponent::OnRunAway (float /*t*/, float /*dt*/)
{
	Move (20.0f, false);
}