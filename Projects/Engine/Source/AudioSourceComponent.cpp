#include "AudioSourceComponent.h"
#include "AudioManager.h"
#include "ErrorMessage.h"
#include <iostream>
#include "Ogre.h"
#include "GameObject.h"
#include "TransformComponent.h"


namespace Engine {

unsigned char AudioSourceComponent::s_instanceCount = 0;


AudioSourceComponent::AudioSourceComponent (const std::string& sourceName, AudioType type):
	Component (sourceName),
	m_type (type),
	m_volume (1.0f),
	m_speed (1.0f),
	m_isLooping (false),
	m_ignorable (false)
{
	s_instanceCount++;

	if (s_instanceCount < AudioManager::GetInstance ().GetMaxSourceCount ())
		alGenSources (1, &m_source);
	else 
		m_ignorable = true;
}


void AudioSourceComponent::PostInit (GameObject* owner)
{
	if (m_ignorable && owner != nullptr)
		owner->RemoveComponent (m_name);
}


void AudioSourceComponent::PostUpdate (float /*t*/, float /*dt*/)
{
	if (m_owner != nullptr) {
		Ogre::Vector3 pos = m_owner->Transform ()->GetGlobalPosition ();
		Ogre::Vector3 dir = m_owner->Transform ()->Forward ();

		if (m_type == SoundEffect) {
			alSource3f (m_source, AL_POSITION, pos.x, pos.y, pos.z);
			alSource3f (m_source, AL_VELOCITY, dir.x, dir.y, dir.z);
			alSource3f (m_source, AL_DIRECTION, dir.x, dir.y, dir.z);
		} else {
			alSourcei (m_source, AL_SOURCE_RELATIVE, AL_TRUE);
			alSource3f (m_source, AL_POSITION, 0.0f, 0.0f, 0.0f);
		}
	}
}


void AudioSourceComponent::Destroy ()
{
	Stop ();
	if (!m_ignorable)
		alDeleteSources (1, &m_source);
}


void AudioSourceComponent::AddBuffer (const std::string& bufferName)
{
	ALuint buffer;
	if (AudioManager::GetInstance ().GetBuffer (bufferName, &buffer)) {
		m_buffers.push_back (buffer);
	}
}


void AudioSourceComponent::Play ()
{
	if (IsPlaying ())
		return;

	// randomizing which buffer to be used
	size_t bufIdx = rand () % m_buffers.size ();
	alSourcei (m_source, AL_BUFFER, m_buffers[bufIdx]);

	if (m_type == SoundEffect) {
		ALfloat volumeRandomFactor = 0.1f * static_cast<ALfloat> (rand ()) / RAND_MAX + 0.95f;
		ALfloat speedRandomFactor = 0.2f * static_cast<ALfloat> (rand ()) / RAND_MAX + 0.9f;

		alSourcef (m_source, AL_GAIN, m_volume * volumeRandomFactor);
		alSourcef (m_source, AL_PITCH, m_speed * speedRandomFactor);
		
		/*alSourcef (m_source, AL_ROLLOFF_FACTOR, 1);
		alSourcef (m_source, AL_REFERENCE_DISTANCE, 5);
		alSourcef (m_source, AL_MAX_DISTANCE, 10);*/
	} else {
		alSourcef (m_source, AL_GAIN, m_volume);
		alSourcef (m_source, AL_PITCH, m_speed);

		/*alSourcef (m_source, AL_ROLLOFF_FACTOR, 1);
		alSourcef (m_source, AL_REFERENCE_DISTANCE, 5);
		alSourcef (m_source, AL_MAX_DISTANCE, 10);*/
	}
	
	Continue ();
}


void AudioSourceComponent::Pause ()
{
	alSourcePause (m_source);
}


void AudioSourceComponent::Stop ()
{
	alSourceStop (m_source);
}


void AudioSourceComponent::Continue ()
{
	alSourcePlay (m_source);
}


bool AudioSourceComponent::IsPlaying ()
{
	ALenum state;
	alGetSourcei (m_source, AL_SOURCE_STATE, &state);
	
	return state == AL_PLAYING;
}


void AudioSourceComponent::SetVolume (ALfloat volume)
{
	m_volume = volume;
	alSourcef (m_source, AL_GAIN, m_volume);
	if (alGetError () != AL_NO_ERROR)
		ERR_LOG (std::cerr, "Could not set the volume of the OpenAL source.\n");
}


void AudioSourceComponent::SetSpeed (ALfloat speed)
{
	m_speed = speed;
	alSourcef (m_source, AL_PITCH, m_speed);
	if (alGetError () != AL_NO_ERROR)
		ERR_LOG (std::cerr, "Could not set the speed of the OpenAL source.\n");
}


void AudioSourceComponent::SetLooping (bool looping)
{
	m_isLooping = looping;

	if (m_isLooping/* && m_type != SoundEffect*/)
		alSourcei (m_source, AL_LOOPING, AL_TRUE);
	else
		alSourcei (m_source, AL_LOOPING, AL_FALSE);

	if (alGetError () != AL_NO_ERROR)
		ERR_LOG (std::cerr, "Could not set the looping of the OpenAL source.\n");
}

}	// namespace Engine
