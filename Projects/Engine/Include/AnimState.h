#pragma once

#ifndef ANIM_STATE_H
#define ANIM_STATE_H

// ---------------------------------- includes ----------------------------------

#include "IState.h"
#include <map>
#include <memory>


namespace Ogre {
class AnimationState;
}

namespace Engine {

// ============================== class AnimState ===============================

class AnimState : public IState
{
public:
	using SPtr = std::shared_ptr<AnimState>;

	explicit DLL_EXPORT 				AnimState (const std::string& name);
	virtual	DLL_EXPORT 					~AnimState ();

	DLL_EXPORT void						Enter (Stateable* stateable) override final;
	DLL_EXPORT void						Execute (Stateable* stateable, float t, float dt) override final;
	DLL_EXPORT void						Exit (Stateable* stateable) override final;

	DLL_EXPORT void						AddAnimState (Ogre::AnimationState* const animState);
	std::weak_ptr<Ogre::AnimationState>	GetAnimState (const std::string& name);

	DLL_EXPORT void						SetWeightAll (float weight);
	DLL_EXPORT void						EnableAll ();

protected:
	using AnimMap = std::map<std::string, std::shared_ptr<Ogre::AnimationState>>;

	AnimMap m_animMap;


	virtual	DLL_EXPORT void PostEnter (Stateable* stateable);
	virtual DLL_EXPORT void PostExecute (Stateable* stateable, float t, float dt);
	virtual DLL_EXPORT void PostExit (Stateable* stateable);

	DLL_EXPORT bool			Blend ( const SPtr& destAnim,
									float srcMinProgression = 0.0f,
									float srcEndWeight = 0.0f,
									float destEndWeight = 1.0f,
									float dWeight = 0.025f);

	float					GetProgression ();
	float					GetWeight ();
	DLL_EXPORT bool			HasEnded (const std::string& name);

	void					IncreaseWeightAll (float weight);
	void					DecreaseWeightAll (float weight);

	void					Enable (const std::string& animName);
	void					Step (const std::string& animName, float dt);
	void					Disable (const std::string& animName);

	void					StepAll (float dt);
	void					DisableAll ();
};

}	// namespace Engine

#endif	// #ifndef ANIM_STATE_H
