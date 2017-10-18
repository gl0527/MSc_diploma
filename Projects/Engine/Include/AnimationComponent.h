#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include <Ogre.h>


namespace Engine
{

// ========================== class AnimationComponent ==========================

class DLL_SPEC AnimationComponent : public Component
{
public:
	AnimationComponent (const std::string& name, const Ogre::SkeletonAnimationBlendMode& blendMode);
	~AnimationComponent ();

	virtual void Start () override;

	void init (const std::string& animName);
	void start ();
	void setEnabled (bool enable);
	void stop ();
	void setWeight (unsigned int weight);
	void setLoop (bool loop);
	bool getLoop ();
	void setTime (float time);
	void step (float time);
	bool ended ();

private:
	bool hasSkeleton;
	Ogre::Entity* entity;
	Ogre::AnimationState* anim;
	Ogre::SkeletonAnimationBlendMode animBlend;
};

}	// namespace Engine

#endif	// #ifndef ANIMATION_COMPONENT_H
