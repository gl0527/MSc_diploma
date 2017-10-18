#ifndef ANIM_STATE_H
#define ANIM_STATE_H

#pragma once



#include "IState.h"
#include "Ogre.h"

namespace Engine
{



// az allapotok blendelesevel talan ezen a szinten kellene foglalkozni
class DLL_SPEC AnimState : public IState
{
public:
	explicit AnimState (const std::string& name) : IState (name) {}

	virtual void Enter (Stateable* stateable) override;
	virtual void Execute (Stateable* stateable, float t, float dt) override;
	virtual void Exit (Stateable* stateable) override;

	void AddAnimState (Ogre::AnimationState* const animState);
	std::weak_ptr<Ogre::AnimationState> GetAnimState (const std::string& name);

protected:
	using AnimMap = std::map <std::string, std::shared_ptr<Ogre::AnimationState>>;

	AnimMap m_animMap;

	void Enable (const std::string& animName);
	void Step (const std::string& animName, float dt);
	void Disable (const std::string& animName);

	void EnableAll ();
	void StepAll (float dt);
	void DisableAll ();

	bool HasEnded (const std::string& name);
};

}	// namespace Engine

#endif	// #ifndef ANIM_STATE_H
