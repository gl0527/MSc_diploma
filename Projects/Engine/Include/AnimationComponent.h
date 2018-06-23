#pragma once

#ifndef	ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

// ---------------------------------- includes ----------------------------------

#include "Component.h"


namespace Ogre {

class Entity;

}	// namespace Ogre

namespace Engine {

// ========================== class AnimationComponent ==========================

class AnimationComponent : public Component
{
public:
	DLL_EXPORT explicit AnimationComponent (const std::string& name);
	DLL_EXPORT virtual ~AnimationComponent ();

	DLL_EXPORT void Transition (const char* fromAnimName, float fromWeight, const char* toAnimName, float toWeight, bool isLooping, float startTime = 0.0f);
	DLL_EXPORT void Enable (const char* animName, bool isLooping, float finalWeight = 1.0f, float startTime = 0.0f);
	DLL_EXPORT void Disable (const char* animName, float finalWeight = 0.0f);
	DLL_EXPORT void Step (const char* animName, float amount);
	DLL_EXPORT bool HasEnded (const char* animName);
	DLL_EXPORT void SetTimePositionInSeconds (const char* animName, float timePos);
	DLL_EXPORT float GetTimePositionInSeconds (const char* animName);
	DLL_EXPORT float GetProgression (const char* animName);
	DLL_EXPORT bool IsLooping (const char* animName);

protected:
	Ogre::Entity* m_ownerEntity;
};

}	// namespace Engine

#endif	// #ifndef	ANIMATION_COMPONENT_H
