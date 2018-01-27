#include "AudioSystem.h"
#include <iostream>


namespace Engine {

AudioSystem::AudioSystem ()
	: m_pAudioDevice (nullptr),
	m_pAudioContext (nullptr),
	m_isEnabled (true)
{
}


AudioSystem::~AudioSystem ()
{
}


bool AudioSystem::init ()
{
	if (!alutInitWithoutContext (0, 0))
		return false;

	m_pAudioDevice = alcOpenDevice (NULL);
	if (!m_pAudioDevice)
		return false;

	m_pAudioContext = alcCreateContext (m_pAudioDevice, NULL);
	alcMakeContextCurrent (m_pAudioContext);
	if (!m_pAudioContext)
		return false;

	ALenum error = alutGetError ();
	if (error != AL_NO_ERROR)
		return false;
	return true;
}


void AudioSystem::destroy ()
{
	alcMakeContextCurrent (NULL);
	alcDestroyContext (m_pAudioContext);
	alcCloseDevice (m_pAudioDevice);
	alutExit ();
}


bool AudioSystem::IsEnabled () const
{
	return m_isEnabled;
}


void AudioSystem::Enable ()
{
	m_isEnabled = true;
}


void AudioSystem::Disable ()
{
	m_isEnabled = false;
}

}	// namespace Engine

