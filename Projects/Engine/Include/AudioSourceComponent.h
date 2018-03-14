#ifndef AUDIO_SOURCE_COMPONENT_H
#define AUDIO_SOURCE_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include <vector>
#include <AL/alut.h>>


namespace Engine {

// ========================= class AudioSourceComponent ==========================

class DLL_EXPORT AudioSourceComponent : public Component
{
public:
	enum AudioType
	{
		SoundEffect = 0,	// 3D, it has the highest priority
		Ambient = 1,	// mostly 2D
		Music = 2	// 2D
	};

	AudioSourceComponent (const std::string& sourceName, AudioType type);

	void PostInit (GameObject* owner) override;
	void PostUpdate (float t, float dt) override;
	void Destroy () override;

	void AddBuffer (const std::string& bufferName);
	
	void Play ();
	void Pause ();
	void Stop ();
	void Continue ();
	bool IsPlaying ();
	
	void SetVolume (ALfloat volume);
	void SetSpeed (ALfloat speed);
	void SetLooping (bool looping);

private:
	static unsigned char	s_instanceCount;

	ALfloat					m_volume;	// +/- 5% random factor at SoundEffect
	ALfloat					m_speed;	// +/- 20% random factor at SoundEffect
	ALfloat					m_isLooping;	// not for SoundEffect
	ALuint					m_source;	// emits audio data, could be bind more than one buffer (binding could be random too)
	std::vector<ALuint>		m_buffers;
	AudioType				m_type;
	bool					m_ignorable;
};

}	// namespace Engine

#endif	// #ifndef AUDIO_SOURCE_COMPONENT_H
