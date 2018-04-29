#ifndef SOLDIER_ANIMATION_COMPONENT_H
#define	SOLDIER_ANIMATION_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include "FiniteStateMachine.h"


using namespace Engine;

namespace Ogre {

class AnimationState;
class Entity;

};

class PlayerDataComponent;

// ====================== class SoldierAnimationComponent =======================

class SoldierAnimationComponent : public Component
{
public:
	enum class UpperBodyState { Idle, Run, WeaponHold, Shoot, Dead };
	enum class LowerBodyState { Idle, Run, Dead };

			SoldierAnimationComponent (const std::string& name);

	void	Start () override;
	void	PreUpdate (float t, float dt) override;

	void	HasWeapon (bool hasWeapon);
	bool	IsDead () const;

private:
	using UpperBodyAnimation = FiniteStateMachine<UpperBodyState, char>;
	using LowerBodyAnimation = FiniteStateMachine<LowerBodyState, char>;

	UpperBodyAnimation						m_upperBodyAnimation;
	LowerBodyAnimation						m_lowerBodyAnimation;
	std::shared_ptr<PlayerDataComponent>	m_ownerData;
	Ogre::Entity*							m_ownerEntity;
	bool									m_isInShootState;
	bool									m_hasWeapon;
	bool									m_isDead;

	void	OnUpperBodyIdle (float t, float dt);
	void	OnUpperBodyRun (float t, float dt);
	void	OnUpperBodyShoot (float t, float dt);
	void	OnUpperBodyWeaponHold (float t, float dt);
	void	OnLowerBodyIdle (float t, float dt);
	void	OnLowerBodyRun (float t, float dt);
	void	OnDeath (float t, float dt);

	void	OnTransition (const char* fromAnimName, const char* toAnimName, bool isLooping);
	void	OnTransitionToDeath ();
	void	TurnOffAnimation (const char* animName);
	void	TurnOnAnimation (const char* animName, bool isLooping);
};

#endif	//#ifndef SOLDIER_ANIMATION_COMPONENT_H
