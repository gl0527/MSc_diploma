#include "AnimState.h"
#include "OgreAnimationState.h"
#include "OgrePrerequisites.h"


namespace Engine {

void AnimState::Enable (const std::string& animName)
{
	if (auto animation = GetAnimState (animName).lock ()) {
		animation->setTimePosition (0.0f);
		animation->setEnabled (true);
	}
}


void AnimState::Step (const std::string& animName, float dt)
{
	if (auto animation = GetAnimState (animName).lock ()) {
		animation->addTime (dt);
	}
}


void AnimState::Disable (const std::string& animName)
{
	if (auto animation = GetAnimState (animName).lock ()) {
		animation->setEnabled (false);
	}
}


void AnimState::EnableAll ()
{
	for (const auto& elem : m_animMap) {
		elem.second->setTimePosition (0.0f);
		elem.second->setEnabled (true);
	}
}


void AnimState::StepAll (float dt)
{
	for (const auto& elem : m_animMap)
		elem.second->addTime (dt);
}


void AnimState::DisableAll ()
{
	for (const auto& elem : m_animMap)
		elem.second->setEnabled (false);
}


bool AnimState::HasEnded (const std::string& animName)
{
	if (auto animation = GetAnimState (animName).lock ()) {
		if (animation->getLoop ())
			return false;
		else if (animation->hasEnded ())
			return true;
	}

	return false;
}


void AnimState::Enter (Stateable* stateable)
{
	EnableAll ();
	PostEnter (stateable);
}


void AnimState::Execute (Stateable* stateable, float t, float dt)
{
	StepAll (dt);
	PostExecute (stateable, t, dt);
}


void AnimState::Exit (Stateable* stateable)
{
	DisableAll ();
	PostExit (stateable);
}


void AnimState::PostEnter (Stateable* stateable)
{
}


void AnimState::PostExecute (Stateable* stateable, float t, float dt)
{
}


void AnimState::PostExit (Stateable* stateable)
{
}


void AnimState::AddAnimState (Ogre::AnimationState* const animState)
{
	m_animMap[animState->getAnimationName ()] = std::make_shared<Ogre::AnimationState> (*animState);
}


std::weak_ptr<Ogre::AnimationState> AnimState::GetAnimState (const std::string& name)
{
	auto it = m_animMap.find (name);
	if (it != m_animMap.end ())
		return m_animMap[name];

	return std::shared_ptr<Ogre::AnimationState> (nullptr);
}


void AnimState::SetWeightAll (float weight)
{
	for (const auto& elem : m_animMap)
		elem.second->setWeight (weight);
}


float AnimState::GetProgression ()
{
	float elapsedTime = m_animMap.begin ()->second->getTimePosition ();
	float totalTime = m_animMap.begin ()->second->getLength ();

	return elapsedTime / totalTime;
}


float AnimState::GetWeight ()
{
	return m_animMap.begin ()->second->getWeight ();
}


void AnimState::IncreaseWeightAll (float weight)
{
	for (const auto& elem : m_animMap) {
		Ogre::Real newWeght (elem.second->getWeight () + weight);
		elem.second->setWeight (newWeght);
	}
}


void AnimState::DecreaseWeightAll (float weight)
{
	for (const auto& elem : m_animMap)
		elem.second->setWeight (elem.second->getWeight () - weight);
}


bool AnimState::Blend (const AnimState::SPtr& destAnim,
	float srcMinProgression /*= 0.0f*/,
	float srcEndWeight /*= 0.0f*/,
	float destEndWeight /*= 1.0f*/,
	float dWeight /*= 0.025f*/)
{
	if (GetProgression () < srcMinProgression)
		return false;

	if (destAnim->GetWeight () > destEndWeight - dWeight &&
		GetWeight () < srcEndWeight + dWeight) {
		return true;
	} else {
		destAnim->IncreaseWeightAll (dWeight);
		DecreaseWeightAll (dWeight);

		return false;
	}
}

}	// namespace Engine
