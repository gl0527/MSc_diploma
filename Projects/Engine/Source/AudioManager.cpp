#include "AudioManager.h"
#include <iostream>
#include "OgreVector3.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ObjectManager.h"


namespace Engine {

AudioManager::AudioManager () :
	m_pAudioDevice (nullptr),
	m_pAudioContext (nullptr),
	m_pathToBuffers (""),
	m_isInitialized (false),
	m_isEnabled (true)
{
	for (size_t i = 0; i < s_MaxSourceCount; ++i) {
		m_sourceIDs[i] = 0;
		m_sourcesInUse[i] = false;
		AL_SAFE_CALL (alSourceStop (m_sourceIDs[i]), "Unable to stop OpenAL source.");
	}
}


AudioManager& AudioManager::GetInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (new AudioManager); });
	return *s_pInstance.get ();
}


void AudioManager::Init ()
{
	if (m_isInitialized)
		return;
	
	if (!alutInitWithoutContext (nullptr, nullptr))
		ERR_THROW (std::runtime_error, "Could not initialize alut!");

	m_pAudioDevice = alcOpenDevice (nullptr);
	if (m_pAudioDevice == nullptr)
		ERR_THROW (std::runtime_error, "Could not create audio device!");

	m_pAudioContext = alcCreateContext (m_pAudioDevice, nullptr);
	if (m_pAudioContext == nullptr)
		ERR_THROW (std::runtime_error, "Could not create audio context!");

	alcMakeContextCurrent (m_pAudioContext);
	if (m_pAudioContext == nullptr)
		ERR_THROW (std::runtime_error, "Could not create audio context!");

	ALenum error = alutGetError ();
	if (error != AL_NO_ERROR)
		ERR_THROW (std::runtime_error, "An error occurred during OpenAL initialization!");

	AL_SAFE_CALL (alGenSources (s_MaxSourceCount, m_sourceIDs), "Unable to create OpenAL sources.");
	AL_SAFE_CALL (alDistanceModel (AL_LINEAR_DISTANCE_CLAMPED), "Unable to set distance model of OpenAL.");

	m_isInitialized = true;
}


void AudioManager::Update ()
{
	if (m_pListenerObj != nullptr) {
		TransformComponent* listenerTransform = m_pListenerObj->Transform ();

		const Ogre::Vector3& listenerPos = listenerTransform->GetGlobalPosition ();
		const Ogre::Vector3& listenerDir = listenerTransform->Forward ();
		const Ogre::Vector3& listenerUp = listenerTransform->Up ();

		AL_SAFE_CALL (alListener3f (AL_POSITION, listenerPos.x, listenerPos.y, listenerPos.z), "Unable to set position of OpenAL listener.");
		AL_SAFE_CALL (alListener3f (AL_VELOCITY, listenerDir.x, listenerDir.y, listenerDir.z), "Unable to set velocity of OpenAL listener.");
		float listenerOrient[] = { listenerDir.x, listenerDir.y, listenerDir.z, listenerUp.x, listenerUp.y, listenerUp.z };
		AL_SAFE_CALL (alListenerfv (AL_ORIENTATION, listenerOrient), "Unable to set orientation of OpenAL listener.");
	}
}


void AudioManager::Destroy ()
{
	AL_SAFE_CALL (alDeleteSources (s_MaxSourceCount, m_sourceIDs), "Unable to delete OpenAL sources.");
	for (auto it = m_bufferIDs.begin (), itEnd = m_bufferIDs.end (); it != itEnd; ++it) {
		unsigned int buffer = it->second;
		AL_SAFE_CALL (alDeleteBuffers (1, &buffer), "Unable to delete OpenAL buffer.");
	}
	alcMakeContextCurrent (nullptr);
	alcDestroyContext (m_pAudioContext);
	alcCloseDevice (m_pAudioDevice);
	alutExit ();
}


void AudioManager::GetBuffer (const std::string& bufferName, unsigned int* outBufferID)
{
	auto it = m_bufferIDs.find (bufferName);
	
	if (it != m_bufferIDs.end ()) {
		*outBufferID = m_bufferIDs.at (bufferName);
	} else {
		unsigned int buffer;
		
		ALUT_SAFE_CALL (buffer = alutCreateBufferFromFile ((m_pathToBuffers + bufferName).c_str ()),
			"Cannot load audio file: " + bufferName);
		
		m_bufferIDs[bufferName] = buffer;
		*outBufferID = buffer;
	}
}


bool AudioManager::GetAvailableSource (unsigned int* outSrcID)
{
	for (unsigned char i = 0; i < s_MaxSourceCount; ++i) {
		if (!m_sourcesInUse[i] && !IsPlaying (m_sourceIDs[i])) {
			m_sourcesInUse[i] = true;
			*outSrcID = m_sourceIDs[i];

			return true;
		}
	}

	return false;
}


bool AudioManager::IsInitialized () const
{
	return m_isInitialized;
}


bool AudioManager::IsEnabled () const
{
	return m_isEnabled;
}


void AudioManager::Enable ()
{
	m_isEnabled = true;
}


void AudioManager::Disable ()
{
	m_isEnabled = false;
}


void AudioManager::UnleashSource (unsigned int sourceID)
{
	for (size_t i = 0; i < s_MaxSourceCount; ++i) {
		if (m_sourceIDs[i] == sourceID)
			m_sourcesInUse[i] = false;
	}
}


void AudioManager::SetPathToBuffers (const std::string& pathToBuffers)
{
	m_pathToBuffers = pathToBuffers;
}


void AudioManager::SetListener (const std::string& listenerName)
{
	m_pListenerObj = ObjectManager::GetInstance ().GetGameObjectByName (listenerName).lock ();
}


bool AudioManager::IsPlaying (unsigned int sourceID) const
{
	ALenum state;
	AL_SAFE_CALL (alGetSourcei (sourceID, AL_SOURCE_STATE, &state), "Unable to get state of OpenAL source.");
	return state == AL_PLAYING;
}


bool AudioManager::IsPaused (unsigned int sourceID) const
{
	ALenum state;
	AL_SAFE_CALL (alGetSourcei (sourceID, AL_SOURCE_STATE, &state), "Unable to get state of OpenAL source.");
	return state == AL_PAUSED;
}


bool AudioManager::IsStopped (unsigned int sourceID) const
{
	ALenum state;
	AL_SAFE_CALL (alGetSourcei (sourceID, AL_SOURCE_STATE, &state), "Unable to get state of OpenAL source.");
	return state == AL_STOPPED;
}

}	// namespace Engine
