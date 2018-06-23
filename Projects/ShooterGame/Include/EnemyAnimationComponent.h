#pragma once

#ifndef ENEMY_ANIMATOIN_COMPONENT_H
#define	ENEMY_ANIMATOIN_COMPONENT_H

// ---------------------------------- includes ----------------------------------

#include "AnimationComponent.h"
#include "FiniteStateMachine.h"


using namespace Engine;


// ----------------------------- forward declaration -----------------------------

class EnemyAIComponent;
class ManagerComponent;

namespace Engine {

class AudioSourceComponent;

}	// namespace Engine

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

	const char* const						m_WalkAnimName;
	const char* const						m_AttackAnimName;
	const char* const						m_DeadAnimName;
	EnemyAnimation							m_animationGraph;
	std::shared_ptr<EnemyAIComponent>		m_ownerAI;
	std::shared_ptr<AudioSourceComponent>	m_ownerAudio;
	std::shared_ptr<ManagerComponent>		m_managerComp;

	void	OnDead (float t, float dt);
};

#endif	// #ifndef ENEMY_ANIMATOIN_COMPONENT_H
