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
private:
	using Tuple = std::tuple<float, float, bool>;

public:
	AudioComponent (const std::string& fileName, const std::string& listenerName);
	virtual ~AudioComponent ();

	virtual void PreUpdate (float t, float dt) override;

	DLL_EXPORT void play ();
	void pause ();
	void stop ();
	bool isPlaying ();

	void setVolume (float vol);
	void setSpeed (float spd);
	void setLooping (bool loop);

	Tuple& GetAttributes ();
	void ApplyAttributes ();

private:
	float volume;
	float speed;
	bool looping;
	ALuint buffer;
	ALuint source;
	std::weak_ptr<GameObject> listener;

	Tuple m_attributes;

	void updatePose (const Ogre::Vector3& pos, const Ogre::Vector3& dir);
};

}	// namespace Engine

#endif	// #ifndef AUDIO_COMPONENT_H