#include "EnemyAnimationComponent.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "OgreEntity.h"
#include "OgreSkeletonInstance.h"
#include "EnemyAIComponent.h"
#include "ObjectManager.h"


EnemyAnimationComponent::EnemyAnimationComponent (const std::string& name, const char* walkAnimName, const char* attackAnimName, const char* deadAnimName):
	AnimationComponent (name),
	m_WalkAnimName (walkAnimName),
	m_AttackAnimName (attackAnimName),
	m_DeadAnimName (deadAnimName),
	m_animationGraph (State::Walk),
	m_ownerAI (nullptr)
{
	m_animationGraph.AddTransitions ({
		{State::Walk, State::Attack, 'a', [this] { Transition (m_WalkAnimName, 0.0f, m_AttackAnimName, 1.0f, true); }},
		{State::Attack, State::Walk, 'w', [this] { Transition (m_AttackAnimName, 0.0f, m_WalkAnimName, 1.0f, true); }},
		{State::Walk, State::Dead, 'd', [this] { Transition (m_WalkAnimName, 0.0f, m_DeadAnimName, 1.0f, false); }},
		{State::Attack, State::Dead, 'd', [this] { Transition (m_AttackAnimName, 0.0f, m_DeadAnimName, 1.0f, false); }}
	});

	m_animationGraph.AddStateFunction (State::Walk, [this] (float t, float dt) { Step (m_WalkAnimName, dt); });
	m_animationGraph.AddStateFunction (State::Attack, [this] (float t, float dt) { Step (m_AttackAnimName, dt); });
	m_animationGraph.AddStateFunction (State::Dead, std::bind (&EnemyAnimationComponent::OnDead, this, std::placeholders::_1, std::placeholders::_2));
}


void EnemyAnimationComponent::Start ()
{
	if (auto ownerAI = m_owner->GetFirstComponentByType<EnemyAIComponent> ().lock ())
		m_ownerAI = ownerAI;

	if (m_ownerAI == nullptr) {
		m_owner->RemoveComponent (m_name);

		return;
	}

	if (auto ownerMesh = m_owner->GetFirstComponentByType<MeshComponent> ().lock ()) {
		m_ownerEntity = ownerMesh->GetEntity ();

		if (auto skeleton = m_ownerEntity->getSkeleton ()) {
			skeleton->setBlendMode (Ogre::ANIMBLEND_CUMULATIVE);

			Enable (m_WalkAnimName, true);

			return;
		}
	}
	m_owner->RemoveComponent (m_name);
}


void EnemyAnimationComponent::PostUpdate (float t, float dt)
{
	if (m_ownerAI->GetState () == EnemyAIComponent::State::Dead)
		m_animationGraph.Process ('d');
	else
		m_animationGraph.Process (m_ownerAI->GetState () == EnemyAIComponent::State::Attack ? 'a' : 'w');

	m_animationGraph.Update (t, dt);
}


void EnemyAnimationComponent::OnDead (float t, float dt)
{
	Step (m_DeadAnimName, dt);
	if (HasEnded (m_DeadAnimName))
		ObjectManager::GetInstance ().MarkGameObjectForDelete (m_owner->GetName ());
}
