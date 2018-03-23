#include "AudioManager.h"
#include "ErrorMessage.h"
#include <iostream>
#include "OgreVector3.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ObjectManager.h"


namespace Engine {

AudioManager::AudioManager () :
	m_pAudioDevice (nullptr),
	m_pAudioContext (nullptr),
	m_pathToBuffers ("")
{
	for (size_t i = 0; i < s_MaxSourceCount; ++i) {
		m_sourceIDs[i] = 0;
	}
}


AudioManager& AudioManager::GetInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (new AudioManager); });
	return *s_pInstance.get ();
}


void AudioManager::Init ()
{
	if (!alutInitWithoutContext (nullptr, nullptr))
		ERR_THROW (std::runtime_error, "Could not initialize alut!");

	m_pAudioDevice = alcOpenDevice (nullptr);
	if (m_pAudioDevice == nullptr)
		ERR_THROW (std::runtime_error, "Could not create audio device!");

	m_pAudioContext = alcCreateContext (m_pAudioDevice, nullptr);
	alcMakeContextCurrent (m_pAudioContext);
	if (m_pAudioContext == nullptr)
		ERR_THROW (std::runtime_error, "Could not create audio context!");

	ALenum error = alutGetError ();
	if (error != AL_NO_ERROR)
		ERR_THROW (std::runtime_error, "An error occurred during OpenAL initialization!");

	alGenSources (s_MaxSourceCount, m_sourceIDs);
	alDistanceModel (AL_LINEAR_DISTANCE_CLAMPED);
}


void AudioManager::Update ()
{
	if (m_pListenerObj != nullptr) {
		TransformComponent* listenerTransform = m_pListenerObj->Transform ();

		const Ogre::Vector3& listenerPos = listenerTransform->GetGlobalPosition ();
		const Ogre::Vector3& listenerDir = listenerTransform->Forward ();
		const Ogre::Vector3& listenerUp = listenerTransform->Up ();

		alListener3f (AL_POSITION, listenerPos.x, listenerPos.y, listenerPos.z);
		alListener3f (AL_VELOCITY, listenerDir.x, listenerDir.y, listenerDir.z);
		float listenerOrient[] = { listenerDir.x, listenerDir.y, listenerDir.z, listenerUp.x, listenerUp.y, listenerUp.z };
		alListenerfv (AL_ORIENTATION, listenerOrient);
	}
}


void AudioManager::Destroy ()
{
	alDeleteSources (s_MaxSourceCount, m_sourceIDs);
	for (auto it = m_bufferIDs.begin (), itEnd = m_bufferIDs.end (); it != itEnd; ++it) {
		unsigned int buffer = it->second;
		alDeleteBuffers (1, &buffer);
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
		unsigned int buffer = alutCreateBufferFromFile ((m_pathToBuffers + bufferName).c_str ());
		m_bufferIDs[bufferName] = buffer;
		*outBufferID = buffer;
	}
}


bool AudioManager::GetAvailableSource (unsigned int* outSrcID) const
{
	for (unsigned char i = 0; i < s_MaxSourceCount; ++i) {
		if (!IsPlaying (m_sourceIDs[i])) {
			*outSrcID = m_sourceIDs[i];

			return true;
		}
	}

	return false;
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
	alGetSourcei (sourceID, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

}	// namespace Engine
