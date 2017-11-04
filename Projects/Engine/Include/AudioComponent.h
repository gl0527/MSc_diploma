#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"

#include <AL\alut.h>
#include <memory>

namespace Ogre
{
	class Vector3;
}


namespace Engine
{

// ============================ class AudioComponent ============================

class DLL_EXPORT AudioComponent : public Component
{
public:
	AudioComponent (const std::string& fileName, const std::string& listenerName);
	virtual ~AudioComponent ();

	virtual void PreUpdate (float t, float dt) override;

	void play ();
	void pause ();
	void stop ();
	bool isPlaying ();

	void setVolume (float vol);
	void setSpeed (float spd);
	void setLooping (bool loop);

private:
	float volume;
	float speed;
	bool looping;
	ALuint buffer;
	ALuint source;
	std::weak_ptr<GameObject> listener;
	void updatePose (const Ogre::Vector3& pos, const Ogre::Vector3& dir);
};

}	// namespace Engine

#endif	// #ifndef AUDIO_COMPONENT_H
