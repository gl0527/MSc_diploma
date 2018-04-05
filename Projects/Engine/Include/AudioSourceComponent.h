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
		SoundEffect = 0,	// 3D, it has the lowest priority
		Ambient = 1,	// mostly 2D
		Music = 2	// 2D
	};

						AudioSourceComponent (const std::string& sourceName, AudioType type);

	void				PostUpdate (float t, float dt) override;
	void				Destroy () override;

	DLL_EXPORT void		AddBuffer (const std::string& bufferName);
	
	DLL_EXPORT void		Play ();
	void				Pause ();
	void				Stop ();
	void				Continue ();

	bool				IsBufferRandomlySelected () const;
	AudioType			GetType () const;

	void				SetSource (unsigned int sourceID);
	DLL_EXPORT void		SetVolume (float volume);
	void				SetSpeed (float speed);
	DLL_EXPORT void		SetLooping (bool looping);
	DLL_EXPORT void		SetMaxDistanceWithFullGain (float refDist);
	DLL_EXPORT void		SetMinDistanceWithZeroGain (float maxDist);
	void				SetRandomBuffers (bool enable);

private:
	float						m_volume;
	float						m_speed;
	float						m_isLooping;
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
