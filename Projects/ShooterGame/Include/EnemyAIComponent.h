#pragma once

#ifndef ENEMY_AI_COMPONENT_H
#define ENEMY_AI_COMPONENT_H

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include "FiniteStateMachine.h"


// ----------------------------- forward declaration -----------------------------

namespace Engine {

class GameObject;
class PhysicsComponent;

}	// namespace Engine

namespace Ogre {

class Vector3;

}	// namespace Ogre

class PlayerDataComponent;

using namespace Engine;

// =========================== class EnemyAIComponent ===========================

class EnemyAIComponent : public Component
{
public:
	enum class State { Search, Attack, RunAway, Dead };

	explicit	EnemyAIComponent (const std::string& name);

	void		Start () override;
	void		PreUpdate (float t, float dt) override;

	State		GetState () const;

private:
	using EnemyStateMachine = FiniteStateMachine<State, char>;
	
	EnemyStateMachine						m_enemyStateMachine;
	std::shared_ptr<GameObject>				m_targetObj;
	std::shared_ptr<PhysicsComponent>		m_pOwnerPhysics;
	std::shared_ptr<PlayerDataComponent>	m_ownerData;

	const Ogre::Vector3&	GetTargetPosition () const;
	const Ogre::Vector3&	GetOwnerPosition () const;
	Ogre::Vector3			GetOwnerFacing () const;

	void Move (float distance, bool follow);

	void OnSearch (float t, float dt);
	void OnAttack (float t, float dt);
	void OnRunAway (float t, float dt);

	void OnCollisionWithPlayer (PhysicsComponent* other);
};

#endif	// #ifndef ENEMY_AI_COMPONENT_H
