#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"

#include <AL\alut.h>
#include <memory>


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
		Descriptor ();

		std::string fileName;
		std::string	listenerName;
		
		float		volume;
		float		speed;
		bool		loop;
	};

					AudioComponent (const std::string& fileName, const std::string& listenerName);
					AudioComponent (const Descriptor& desc);
	virtual			~AudioComponent ();

	virtual void	PreUpdate (float t, float dt) override;

	DLL_EXPORT void Play ();
	void			Pause ();
	void			Stop ();
	bool			IsPlaying ();

	void			SetVolume (float vol);
	void			SetSpeed (float spd);
	void			SetLooping (bool loop);

	void			ApplyDescriptor (const Descriptor& desc);

private:
	float						m_volume;
	float						m_speed;
	bool						m_looping;
	ALuint						m_buffer;
	ALuint						m_source;
	std::weak_ptr<GameObject>	m_pListener;

	void			UpdatePose (const Ogre::Vector3& pos, const Ogre::Vector3& dir);
};

}	// namespace Engine

#endif	// #ifndef AUDIO_COMPONENT_H
