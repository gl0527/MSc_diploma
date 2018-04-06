#ifndef AUDIO_SOURCE_COMPONENT_H
#define AUDIO_SOURCE_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include <vector>
#include "AudioManager.h"


namespace Engine {

// ========================= class AudioSourceComponent ==========================

class AudioSourceComponent : public Component
{
public:
	enum AudioType
	{
		SoundEffect,
		Ambient,
		Music
	};

	struct Descriptor
	{
		Descriptor ();

		std::string name;
		AudioType	audioType;

		float						volume;
		float						speed;
		bool						isLooping;
		float						maxDistWithFullGain;
		float						minDistWithZeroGain;
		bool						isBufferRandomlySelected;
		std::vector<std::string>	buffers;
	};

						AudioSourceComponent (const std::string& sourceName, AudioType type);
						AudioSourceComponent (const Descriptor& desc);

	void				PostUpdate (float t, float dt) override;
	void				Destroy () override;

	DLL_EXPORT void		AddBuffer (const std::string& bufferName);
	
	DLL_EXPORT void		Play ();
	void				Pause ();
	void				Stop ();
	void				Continue ();

	bool				IsBufferRandomlySelected () const;
	AudioType			GetType () const;

	void				BindSource (unsigned int sourceID);
	void				UnBindSource ();
	DLL_EXPORT void		SetVolume (float volume);
	void				SetSpeed (float speed);
	DLL_EXPORT void		SetLooping (bool looping);
	DLL_EXPORT void		SetMaxDistanceWithFullGain (float refDist);
	DLL_EXPORT void		SetMinDistanceWithZeroGain (float maxDist);
	void				SetRandomBuffers (bool enable);

	void				ApplyDescriptor (const Descriptor& desc);

private:
	float						m_volume;
	float						m_speed;
	bool						m_isLooping;
	float						m_maxDistWithFullGain;
	float						m_minDistWithZeroGain;
	bool						m_isBufferRandomlySelected;
	size_t						m_currentBufferIndex;
	unsigned int				m_sourceID;
	std::vector<unsigned int>	m_bindedBufferIDs;
	AudioType					m_type;
	AudioManager&				m_audioManager;


	void				PostInit (GameObject* object) override;
};

}	// namespace Engine

#endif	// #ifndef AUDIO_SOURCE_COMPONENT_H
