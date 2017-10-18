#include "AnimState.h"


namespace Engine
{

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
}


void AnimState::Execute (Stateable* stateable, float t, float dt)
{
	StepAll (dt);
}


void AnimState::Exit (Stateable* stateable)
{
	DisableAll ();
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

}	// namespace Engine
