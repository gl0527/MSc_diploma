#include "AudioSourceComponent.h"
#include "AudioManager.h"
#include <iostream>
#include "OgreVector3.h"
#include "GameObject.h"
#include "TransformComponent.h"


namespace Engine {

AudioSourceComponent::Descriptor::Descriptor ():
	name (""),
	audioType (Music),
	volume (1.0f),
	speed (1.0f),
	isLooping (false),
	maxDistWithFullGain (50.0f),
	minDistWithZeroGain (200.0f),
	isBufferRandomlySelected (true)
{
}


AudioSourceComponent::AudioSourceComponent (const std::string& sourceName, AudioType type):
	Component (sourceName),
	m_type (type),
	m_volume (1.0f),
	m_speed (1.0f),
	m_isLooping (false),
	m_maxDistWithFullGain (50.0f),
	m_minDistWithZeroGain (200.0f),
	m_isBufferRandomlySelected (true),
	m_currentBufferIndex (0),
	m_sourceID (0),
	m_audioManager (AudioManager::GetInstance ())
{
}


AudioSourceComponent::AudioSourceComponent (const Descriptor& desc):
	AudioSourceComponent (desc.name, desc.audioType)
{
}


void AudioSourceComponent::PostInit (GameObject* object)
{
	std::shared_ptr<AudioSourceComponent> spFromThis = std::dynamic_pointer_cast<AudioSourceComponent> (object->GetComponent (m_name).lock ());
	m_audioManager.AddAudioSourceComponent (spFromThis);
}


void AudioSourceComponent::PostUpdate (float /*t*/, float /*dt*/)
{
	if (m_sourceID == 0)
		return;

	if (m_owner != nullptr) {
		Ogre::Vector3 pos = m_owner->Transform ()->GetGlobalPosition ();
		Ogre::Vector3 dir = m_owner->Transform ()->GetGlobalFacing ();

		if (m_type == SoundEffect) {
			AL_SAFE_CALL (alSource3f (m_sourceID, AL_POSITION, pos.x, pos.y, pos.z), "Unable to set position of OpenAL source.");
			AL_SAFE_CALL (alSource3f (m_sourceID, AL_VELOCITY, dir.x, dir.y, dir.z), "Unable to set velocity of OpenAL source.");
			AL_SAFE_CALL (alSource3f (m_sourceID, AL_DIRECTION, dir.x, dir.y, dir.z), "Unable to set direction of OpenAL source.");
		}
	}

	if (m_type != SoundEffect) {
		AL_SAFE_CALL (alSourcei (m_sourceID, AL_SOURCE_RELATIVE, AL_TRUE), "Unable to set OpenAL source relative to the listener.");
		AL_SAFE_CALL (alSource3f (m_sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f), "Unable to set position of OpenAL source.");
	}

	if (!m_audioManager.IsPlaying (m_sourceID) && m_audioManager.IsEnabled ()) {
		if (m_isLooping) {
			Play ();
		}
	}
}


void AudioSourceComponent::Destroy ()
{
	Stop ();
	std::shared_ptr<AudioSourceComponent> spFromThis = std::dynamic_pointer_cast<AudioSourceComponent> (m_owner->GetComponent (m_name).lock ());
	m_audioManager.RemoveAudioSourceComponent (spFromThis);
}


void AudioSourceComponent::AddBuffer (const std::string& bufferName)
{
	unsigned int bufferID;
	m_audioManager.GetBuffer (bufferName, &bufferID);
	m_bindedBufferIDs[bufferName] = bufferID;
	m_bindedBufferNames.push_back (bufferName);
}


void AudioSourceComponent::Play (std::string bufferName)
{
	if (m_sourceID == 0)
		return;
	
	if (!m_audioManager.IsInitialized () ||
		!m_audioManager.IsEnabled () ||
		m_audioManager.IsPlaying (m_sourceID))
		return;

	if (bufferName != "") {
		auto it = m_bindedBufferIDs.find (bufferName);
		if (it != m_bindedBufferIDs.end ())
			AL_SAFE_CALL (alSourcei (m_sourceID, AL_BUFFER, m_bindedBufferIDs[bufferName]), "Unable to bind buffer to OpenAL source.");
	} else {
		std::string chosenBuffer;

		if (m_isBufferRandomlySelected) {
			size_t bufIdx = rand () % m_bindedBufferNames.size ();
			chosenBuffer = m_bindedBufferNames[bufIdx];
			AL_SAFE_CALL (alSourcei (m_sourceID, AL_BUFFER, m_bindedBufferIDs[chosenBuffer]), "Unable to bind buffer to OpenAL source.");
		} else {
			chosenBuffer = m_bindedBufferNames[m_currentBufferIndex % m_bindedBufferNames.size ()];
			AL_SAFE_CALL (alSourcei (m_sourceID, AL_BUFFER, m_bindedBufferIDs[chosenBuffer]), "Unable to bind buffer to OpenAL source.");
			++m_currentBufferIndex;
		}

		if (m_type == SoundEffect) {
			float speedRandomFactor = 0.4f * static_cast<float> (rand ()) / RAND_MAX + 0.8f;
			AL_SAFE_CALL (alSourcef (m_sourceID, AL_PITCH, m_speed * speedRandomFactor), "Unable to set pitch of OpenAL source.");
		}
	}

	Continue ();
}


void AudioSourceComponent::Pause ()
{
	if (m_sourceID == 0)
		return;

	AL_SAFE_CALL (alSourcePause (m_sourceID), "Unable to pause OpenAL source.");
}


void AudioSourceComponent::Stop ()
{
	if (m_sourceID == 0)
		return;

	AL_SAFE_CALL (alSourceStop (m_sourceID), "Unable to stop OpenAL source.");
}


void AudioSourceComponent::Continue ()
{
	if (m_sourceID == 0)
		return;

	AL_SAFE_CALL (alSourcePlay (m_sourceID), "Unable to play OpenAL source.");
}


void AudioSourceComponent::BindSource (unsigned int sourceID)
{
	if (m_sourceID == sourceID)
		return;

	m_sourceID = sourceID;

	Stop ();

	SetVolume (m_volume);
	SetSpeed (m_speed);
	SetLooping (m_isLooping);
	SetMaxDistanceWithFullGain (m_maxDistWithFullGain);
	SetMinDistanceWithZeroGain (m_minDistWithZeroGain);
}


void AudioSourceComponent::UnBindSource ()
{
	m_sourceID = 0;
}


void AudioSourceComponent::SetVolume (float volume)
{
	m_volume = volume;

	if (m_sourceID == 0)
		return;

	AL_SAFE_CALL (alSourcef (m_sourceID, AL_GAIN, m_volume), "Unable to set volume of the OpenAL source.");
}


void AudioSourceComponent::SetSpeed (float speed)
{
	m_speed = speed;

	if (m_sourceID == 0)
		return;

	AL_SAFE_CALL (alSourcef (m_sourceID, AL_PITCH, m_speed), "Unable to set pitch of OpenAL source.");
}


void AudioSourceComponent::SetLooping (bool looping)
{
	m_isLooping = looping;
}


void AudioSourceComponent::SetMaxDistanceWithFullGain (float refDist)
{
	if (m_type != SoundEffect)
		return;
	
	m_maxDistWithFullGain = refDist;

	if (m_sourceID == 0)
		return;

	AL_SAFE_CALL (alSourcef (m_sourceID, AL_REFERENCE_DISTANCE, m_maxDistWithFullGain), "Unable to set reference distance of OpenAL source.");
}


void AudioSourceComponent::SetMinDistanceWithZeroGain (float maxDist)
{
	if (m_type != SoundEffect)
		return;
	
	m_minDistWithZeroGain = maxDist;

	if (m_sourceID == 0)
		return;

	AL_SAFE_CALL (alSourcef (m_sourceID, AL_MAX_DISTANCE, m_minDistWithZeroGain), "Unable to set max distance of OpenAL source.");
}


bool AudioSourceComponent::IsBufferRandomlySelected () const
{
	return m_isBufferRandomlySelected;
}


AudioSourceComponent::AudioType AudioSourceComponent::GetType () const
{
	return m_type;
}


void AudioSourceComponent::SetRandomBuffers (bool enable)
{
	m_isBufferRandomlySelected = enable;
}


void AudioSourceComponent::ApplyDescriptor (const AudioSourceComponent::Descriptor& desc)
{
	SetVolume (desc.volume);
	SetSpeed (desc.speed);
	SetLooping (desc.isLooping);
	SetMaxDistanceWithFullGain (desc.maxDistWithFullGain);
	SetMinDistanceWithZeroGain (desc.minDistWithZeroGain);
	SetRandomBuffers (desc.isBufferRandomlySelected);

	for (const std::string& buffer : desc.buffers)
		AddBuffer (buffer);
}

}	// namespace Engine
