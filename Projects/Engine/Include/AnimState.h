#ifndef ANIM_STATE_H
#define ANIM_STATE_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "IState.h"
#include <map>
#include <memory>


namespace Ogre
{
	class AnimationState;
}

namespace Engine
{

// ============================== class AnimState ===============================

class DLL_EXPORT AnimState : public IState
{
public:
	using SPtr = std::shared_ptr<AnimState>;

	explicit AnimState (const std::string& name) : IState (name) {}

	void Enter (Stateable* stateable) override final;
	void Execute (Stateable* stateable, float t, float dt) override final;
	void Exit (Stateable* stateable) override final;

	void AddAnimState (Ogre::AnimationState* const animState);
	std::weak_ptr<Ogre::AnimationState> GetAnimState (const std::string& name);

	void SetWeightAll (float weight);
	void EnableAll ();

protected:
	using AnimMap = std::map <std::string, std::shared_ptr<Ogre::AnimationState>>;

	AnimMap m_animMap;


	virtual void PostEnter (Stateable* stateable);
	virtual void PostExecute (Stateable* stateable, float t, float dt);
	virtual void PostExit (Stateable* stateable);

	bool Blend (const SPtr& destAnim,
		float srcMinProgression = 0.0f,
		float srcEndWeight = 0.0f,
		float destEndWeight = 1.0f,
		float dWeight = 0.015f);

	float GetProgression ();
	float GetWeight ();
	bool HasEnded (const std::string& name);

	void IncreaseWeightAll (float weight);
	void DecreaseWeightAll (float weight);

	void Enable (const std::string& animName);
	void Step (const std::string& animName, float dt);
	void Disable (const std::string& animName);

	void StepAll (float dt);
	void DisableAll ();
};

}	// namespace Engine

#endif	// #ifndef ANIM_STATE_H
