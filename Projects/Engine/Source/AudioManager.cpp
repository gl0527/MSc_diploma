#include "AudioManager.h"
#include "ErrorMessage.h"
#include <iostream>
#include "Ogre.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ObjectManager.h"


namespace Engine {

AudioManager::AudioManager () :
	m_pAudioDevice (nullptr),
	m_pAudioContext (nullptr)
{
	alDistanceModel (AL_LINEAR_DISTANCE_CLAMPED);
}


AudioManager& AudioManager::GetInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (new AudioManager); });
	return *s_pInstance.get ();
}


unsigned char AudioManager::GetMaxSourceCount ()
{
	return s_MaxSourceCount;
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
}


void AudioManager::Update ()
{
	if (m_pListenerObj != nullptr) {
		TransformComponent* listenerTransform = m_pListenerObj->Transform ();

		const Ogre::Vector3& listenerPos = listenerTransform->GetGlobalPosition ();
		const Ogre::Vector3& listenerDir = listenerTransform->Forward ();
		const Ogre::Vector3& listenerUp = listenerTransform->Up ();

		alListener3f (AL_POSITION, listenerPos.x, listenerPos.y, listenerPos.z);
		ALfloat listenerOrient[] = { listenerDir.x, listenerDir.y, listenerDir.z, listenerUp.x, listenerUp.y, listenerUp.z };
		alListenerfv (AL_ORIENTATION, listenerOrient);
	}
}


void AudioManager::Destroy ()
{
	for (auto it = m_buffers.begin (), itEnd = m_buffers.end (); it != itEnd; ++it) {
		ALuint buffer = it->second;
		alDeleteBuffers (1, &buffer);
	}
	alcMakeContextCurrent (nullptr);
	alcDestroyContext (m_pAudioContext);
	alcCloseDevice (m_pAudioDevice);
	alutExit ();
}


void AudioManager::AddBuffer (const std::string& filePath)
{
	ALuint buffer = alutCreateBufferFromFile (filePath.c_str ());
	m_buffers[filePath] = buffer;
}


bool AudioManager::GetBuffer (const std::string& bufferName, ALuint* outBuffer)
{
	auto it = m_buffers.find (bufferName);
	
	if (it != m_buffers.end ()) {
		*outBuffer = m_buffers.at (bufferName);

		return true;
	}

	return false;
}


void AudioManager::SetListener (const std::string& listenerName)
{
	m_pListenerObj = ObjectManager::GetInstance ().GetGameObjectByName (listenerName).lock ();
}

}	// namespace Engine
