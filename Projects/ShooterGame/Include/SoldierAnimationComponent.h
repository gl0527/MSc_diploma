#ifndef SOLDIER_ANIMATION_COMPONENT_H
#define	SOLDIER_ANIMATION_COMPONENT_H

#pragma once

#include "Component.h"
#include "FiniteStateMachine.h"


using namespace Engine;

namespace Ogre {

class AnimationState;
class Entity;

};

class SoldierAnimationComponent : public Component
{
public:
	enum class UpperBodyState { Idle, Run, WeaponHold, Shoot };
	enum class LowerBodyState { Idle, Run };

	SoldierAnimationComponent (const std::string& name);

	void Start () override;
	void PreUpdate (float t, float dt) override;

private:
	void OnUpperBodyIdle (float t, float dt);
	void OnUpperBodyRun (float t, float dt);
	void OnUpperBodyShoot (float t, float dt);
	void OnUpperBodyWeaponHold (float t, float dt);
	void OnLowerBodyIdle (float t, float dt);
	void OnLowerBodyRun (float t, float dt);

	void OnTransition (const char* fromAnimName, const char* toAnimName, bool isLooping);

	using UpperBodyAnimation = FiniteStateMachine<UpperBodyState, char>;
	using LowerBodyAnimation = FiniteStateMachine<LowerBodyState, char>;
	using AnimStateSPtr = std::shared_ptr<Ogre::AnimationState>;
	using EntitySPtr = std::shared_ptr<Ogre::Entity>;

	UpperBodyAnimation m_upperBodyAnimation;
	LowerBodyAnimation m_lowerBodyAnimation;

	Ogre::Entity* m_ownerEntity;
	bool m_isInShootState;
	bool m_hasWeapon;
};




#endif	//#ifndef SOLDIER_ANIMATION_COMPONENT_H
