#ifndef ENEMY_ANIMATOIN_COMPONENT_H
#define	ENEMY_ANIMATOIN_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "AnimationComponent.h"
#include "FiniteStateMachine.h"


using namespace Engine;


// ----------------------------- forward declaration -----------------------------

class EnemyAIComponent;

// ======================= class EnemyAnimationComponent ========================

class EnemyAnimationComponent : public AnimationComponent
{
public:
	enum class State { Walk, Attack, Dead };

			EnemyAnimationComponent (const std::string& name, const char* walkAnimName, const char* attackAnimName, const char* deadAnimName);

	void	Start () override;
	void	PostUpdate (float t, float dt) override;

private:
	using EnemyAnimation = FiniteStateMachine<State, char>;

	bool								m_isDeleted;

	const char* const					m_WalkAnimName;
	const char* const					m_AttackAnimName;
	const char* const					m_DeadAnimName;
	EnemyAnimation						m_animationGraph;
	std::shared_ptr<EnemyAIComponent>	m_ownerAI;

	void	OnDead (float t, float dt);
};

#endif	// #ifndef ENEMY_ANIMATOIN_COMPONENT_H
