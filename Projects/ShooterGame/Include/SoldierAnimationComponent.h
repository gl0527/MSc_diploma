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
	enum class UpperBodyState { Idle, Run, Shoot };
	enum class LowerBodyState { Idle, Run };

	SoldierAnimationComponent (const std::string& name);

	void Start () override;
	void PreUpdate (float t, float dt) override;

private:
	using UpperBodyAnimation = FiniteStateMachine<UpperBodyState, char>;
	using LowerBodyAnimation = FiniteStateMachine<LowerBodyState, char>;

	UpperBodyAnimation m_upperBodyAnimation;
	LowerBodyAnimation m_lowerBodyAnimation;

	Ogre::AnimationState* m_upperAnimState;
	Ogre::AnimationState* m_lowerAnimState;
	Ogre::Entity* m_ownerEntity;
};




#endif	//#ifndef SOLDIER_ANIMATION_COMPONENT_H
