#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"

#include <AL\alut.h>
#include <memory>
#include <tuple>


namespace Ogre {
class Vector3;
}


namespace Engine {

// ============================ class AudioComponent ============================

class AudioComponent : public Component
{
public:
	struct Descriptor
	{
		Descriptor ()
			: fileName (""),
			listenerName (""),
			volume (0.0f),
			speed (0.0f),
			loop (false)
		{
		}

		std::string fileName;
		std::string listenerName;
		
		float volume;
		float speed;
		bool loop;
	};

	AudioComponent (const std::string& fileName, const std::string& listenerName);
	AudioComponent (const Descriptor& desc);
	virtual ~AudioComponent ();

	virtual void PreUpdate (float t, float dt) override;

	DLL_EXPORT void play ();
	void pause ();
	void stop ();
	bool isPlaying ();

	void setVolume (float vol);
	void setSpeed (float spd);
	void setLooping (bool loop);

	void ApplyDescriptor (const Descriptor& desc);

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
