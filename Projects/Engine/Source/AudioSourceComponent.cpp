#include "AudioSourceComponent.h"
#include "AudioManager.h"
#include "ErrorMessage.h"
#include <iostream>
#include "OgreVector3.h"
#include "GameObject.h"
#include "TransformComponent.h"


namespace Engine {

AudioSourceComponent::AudioSourceComponent (const std::string& sourceName, AudioType type):
	Component (sourceName),
	m_type (type),
	m_volume (1.0f),
	m_speed (1.0f),
	m_isLooping (false),
	m_maxDistWithFullGain (50.0f),
	m_minDistWithZeroGain (200.0f),
	m_sourceID (0)
{
}


void AudioSourceComponent::PostUpdate (float /*t*/, float /*dt*/)
{
	if (m_sourceID == 0)
		return;

	if (m_owner != nullptr) {
		Ogre::Vector3 pos = m_owner->Transform ()->GetGlobalPosition ();
		Ogre::Vector3 dir = m_owner->Transform ()->Forward ();

		if (m_type == SoundEffect) {	// 3D
			alSource3f (m_sourceID, AL_POSITION, pos.x, pos.y, pos.z);
			alSource3f (m_sourceID, AL_VELOCITY, dir.x, dir.y, dir.z);
			alSource3f (m_sourceID, AL_DIRECTION, dir.x, dir.y, dir.z);
		} else {	// 2D
			alSourcei (m_sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
			alSource3f (m_sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
		}
	}

	if (!AudioManager::GetInstance ().IsPlaying (m_sourceID)) {
		if (m_isLooping)
			Play ();
		else
			m_sourceID = 0;
	}
}


void AudioSourceComponent::Destroy ()
{
	Stop ();
}


void AudioSourceComponent::AddBuffer (const std::string& bufferName)
{
	unsigned int bufferID;
	AudioManager::GetInstance ().GetBuffer (bufferName, &bufferID);
	m_bindedBufferIDs.push_back (bufferID);
}


void AudioSourceComponent::Play ()
{
	if (m_sourceID == 0) {
		if (!AudioManager::GetInstance ().GetAvailableSource (&m_sourceID)) {
			ERR_LOG (std::cerr, "Audio cannot be played because there is no free audio source in the system.\n");

			return;
		}
	}

	size_t bufIdx = rand () % m_bindedBufferIDs.size ();
	alSourcei (m_sourceID, AL_BUFFER, m_bindedBufferIDs[bufIdx]);

	if (m_type == SoundEffect) {
		float speedRandomFactor = 0.4f * static_cast<float> (rand ()) / RAND_MAX + 0.8f;
		alSourcef (m_sourceID, AL_PITCH, m_speed * speedRandomFactor);
	}

	Continue ();
}


void AudioSourceComponent::Pause ()
{
	if (m_sourceID == 0) {
		if (!AudioManager::GetInstance ().GetAvailableSource (&m_sourceID)) {

			return;
		}
	}

	alSourcePause (m_sourceID);
}


void AudioSourceComponent::Stop ()
{
	if (m_sourceID == 0) {
		if (!AudioManager::GetInstance ().GetAvailableSource (&m_sourceID)) {

			return;
		}
	}
	
	alSourceStop (m_sourceID);
}


void AudioSourceComponent::Continue ()
{
	if (m_sourceID == 0) {
		if (!AudioManager::GetInstance ().GetAvailableSource (&m_sourceID)) {

			return;
		}
	}

	alSourcePlay (m_sourceID);
}


void AudioSourceComponent::SetVolume (float volume)
{
	if (m_type == SoundEffect)
		return;
	
	if (m_sourceID == 0) {
		if (!AudioManager::GetInstance ().GetAvailableSource (&m_sourceID)) {

			return;
		}
	}
	
	m_volume = volume;
	alSourcef (m_sourceID, AL_GAIN, m_volume);
	if (alGetError () != AL_NO_ERROR)
		ERR_LOG (std::cerr, "Could not set the volume of the OpenAL source.\n");
}


void AudioSourceComponent::SetSpeed (float speed)
{
	if (m_sourceID == 0) {
		if (!AudioManager::GetInstance ().GetAvailableSource (&m_sourceID)) {

			return;
		}
	}
	
	m_speed = speed;
	alSourcef (m_sourceID, AL_PITCH, m_speed);
	if (alGetError () != AL_NO_ERROR)
		ERR_LOG (std::cerr, "Could not set the speed of the OpenAL source.\n");
}


void AudioSourceComponent::SetLooping (bool looping)
{
	if (m_sourceID == 0) {
		if (!AudioManager::GetInstance ().GetAvailableSource (&m_sourceID)) {

			return;
		}
	}
	
	m_isLooping = looping;

	//if (m_isLooping/* && m_type != SoundEffect*/)
	//	alSourcei (m_sourceID, AL_LOOPING, AL_TRUE);
	//else
	//	alSourcei (m_sourceID, AL_LOOPING, AL_FALSE);

	//if (alGetError () != AL_NO_ERROR)
	//	ERR_LOG (std::cerr, "Could not set the looping of the OpenAL source.\n");
}


void AudioSourceComponent::SetMaxDistanceWithFullGain (float refDist)
{
	if (m_type != SoundEffect)
		return;
	
	if (m_sourceID == 0) {
		if (!AudioManager::GetInstance ().GetAvailableSource (&m_sourceID)) {

			return;
		}
	}

	alSourcef (m_sourceID, AL_REFERENCE_DISTANCE, refDist);
}


void AudioSourceComponent::SetMinDistanceWithZeroGain (float maxDist)
{
	if (m_type != SoundEffect)
		return;
	
	if (m_sourceID == 0) {
		if (!AudioManager::GetInstance ().GetAvailableSource (&m_sourceID)) {

			return;
		}
	}

	alSourcef (m_sourceID, AL_MAX_DISTANCE, maxDist);
}

}	// namespace Engine
