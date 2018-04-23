#include "SoldierStateComponent.h"
#include <iostream>
#include "ObjectManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "AudioSourceComponent.h"


SoldierStateComponent::SoldierStateComponent (const std::string& name):
	Component (name),
	soldierStateMachine (States::Follow)
{
	soldierStateMachine.AddTransitions ({
		{States::Move_Away, States::Follow, 'F', [] {std::cout << "You are my father and I am your child!\n"; }},
	});
}


void SoldierStateComponent::Start ()
{
	if (auto obj = ObjectManager::GetInstance ().GetGameObjectByName ("gijoe").lock ())
		m_leaderObj = obj;
	
	if (m_leaderObj == nullptr)
		return;

	soldierStateMachine.AddTransitions ({
		{States::Follow, States::Move_Away, 'M', [this] {
		if (auto audio = m_owner->GetFirstComponentByType<AudioSourceComponent> ().lock ()) {
			audio->Play ();
		}
		}},
	});

	soldierStateMachine.AddStateFunction (States::Follow, [this] (float t, float dt) {
		const Ogre::Vector3& leaderWorldPos = m_leaderObj->Transform ()->GetGlobalPosition ();
		const Ogre::Vector3& ownerWorldPos = m_owner->Transform ()->GetGlobalPosition ();

		if (leaderWorldPos.distance (ownerWorldPos) > 30.0f) {
			Ogre::Vector3 direction = (leaderWorldPos - ownerWorldPos);
			direction.normalise ();
			m_owner->Transform ()->Translate (direction);
			m_owner->Transform ()->SetGlobalRotation (m_leaderObj->Transform ()->GetGlobalRotation ());
		}
	});

	soldierStateMachine.AddStateFunction (States::Move_Away, [this] (float t, float dt) {
		const Ogre::Vector3& leaderWorldPos = m_leaderObj->Transform ()->GetGlobalPosition ();
		const Ogre::Vector3& ownerWorldPos = m_owner->Transform ()->GetGlobalPosition ();

		Ogre::Vector3 direction = (ownerWorldPos - leaderWorldPos);
		direction.normalise ();
		m_owner->Transform ()->Translate (direction);
		m_owner->Transform ()->SetGlobalRotation (m_leaderObj->Transform ()->GetGlobalRotation ().Inverse ());
	});
}


void SoldierStateComponent::PreUpdate (float t, float dt)
{
	const Ogre::Vector3& leaderWorldPos = m_leaderObj->Transform ()->GetGlobalPosition ();
	const Ogre::Vector3& ownerWorldPos = m_owner->Transform ()->GetGlobalPosition ();

	if (leaderWorldPos.distance (ownerWorldPos) < 20.0f) {
		soldierStateMachine.Process ('M');
	} else {
		soldierStateMachine.Process ('F');
	}

	soldierStateMachine.Update (t, dt);
}


SoldierStateComponent::States SoldierStateComponent::GetState ()
{
	return soldierStateMachine.GetState ();
}