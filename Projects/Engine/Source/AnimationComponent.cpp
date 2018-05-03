#include "AnimationComponent.h"
#include "OgreAnimationState.h"
#include "OgreEntity.h"


namespace Engine {

AnimationComponent::AnimationComponent (const std::string& name):
	Component (name),
	m_ownerEntity (nullptr)
{
}


AnimationComponent::~AnimationComponent ()
{
}


void AnimationComponent::Transition (const char* fromAnimName, float fromWeight, const char* toAnimName, float toWeight, bool isLooping, float startTime)
{
	Disable (fromAnimName, fromWeight);
	Enable (toAnimName, isLooping, toWeight, startTime);
}


void AnimationComponent::Enable (const char* animName, bool isLooping, float finalWeight, float startTime)
{
	if (m_ownerEntity == nullptr)
		return;

	if (m_ownerEntity->hasAnimationState (animName)) {
		Ogre::AnimationState* animState = m_ownerEntity->getAnimationState (animName);
		animState->setEnabled (true);
		animState->setLoop (isLooping);
		animState->setTimePosition (startTime);
		animState->setWeight (finalWeight);
	}
}


void AnimationComponent::Disable (const char* animName, float finalWeight)
{
	if (m_ownerEntity == nullptr)
		return;

	if (m_ownerEntity->hasAnimationState (animName)) {
		Ogre::AnimationState* animState = m_ownerEntity->getAnimationState (animName);
		animState->setEnabled (false);
		animState->setLoop (false);
		animState->setWeight (finalWeight);
	}
}


void AnimationComponent::Step (const char* animName, float amount)
{
	if (m_ownerEntity == nullptr)
		return;

	if (m_ownerEntity->hasAnimationState (animName)) {
		Ogre::AnimationState* animState = m_ownerEntity->getAnimationState (animName);
		animState->addTime (amount);
	}
}


bool AnimationComponent::HasEnded (const char* animName)
{
	if (m_ownerEntity == nullptr)
		return false;

	if (m_ownerEntity->hasAnimationState (animName)) {
		Ogre::AnimationState* animState = m_ownerEntity->getAnimationState (animName);
		
		return animState->hasEnded ();	// if animState is looping, then the return value will always be false
	}

	return false;
}


void AnimationComponent::SetTimePositionInSeconds (const char* animName, float timePos)
{
	if (m_ownerEntity == nullptr)
		return;

	if (m_ownerEntity->hasAnimationState (animName)) {
		Ogre::AnimationState* animState = m_ownerEntity->getAnimationState (animName);
		animState->setTimePosition (timePos);
	}
}


float AnimationComponent::GetTimePositionInSeconds (const char* animName)
{
	if (m_ownerEntity == nullptr)
		return 0.0f;

	if (m_ownerEntity->hasAnimationState (animName)) {
		Ogre::AnimationState* animState = m_ownerEntity->getAnimationState (animName);
		
		return animState->getTimePosition ();
	}

	return 0.0f;
}


float AnimationComponent::GetProgression (const char* animName)
{
	if (m_ownerEntity == nullptr)
		return 0.0f;

	if (m_ownerEntity->hasAnimationState (animName)) {
		Ogre::AnimationState* animState = m_ownerEntity->getAnimationState (animName);
		
		return animState->getTimePosition () / animState->getLength ();
	}

	return 0.0f;
}


bool AnimationComponent::IsLooping (const char* animName)
{
	if (m_ownerEntity == nullptr)
		return false;

	if (m_ownerEntity->hasAnimationState (animName)) {
		Ogre::AnimationState* animState = m_ownerEntity->getAnimationState (animName);
		
		return animState->getLoop ();
	}

	return false;	
}

}	// namespace Engine