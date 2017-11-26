#include "AudioSystem.h"
#include <iostream>


namespace Engine {

AudioSystem::AudioSystem ()
	: audioDevice (nullptr),
	audioContext (nullptr),
	enabled (true),
	MAX_BUFFERS (32),
	MAX_SOURCES (8)
{
}


AudioSystem::~AudioSystem ()
{
}


bool AudioSystem::init ()
{
	if (!alutInitWithoutContext (0, 0))
		return false;

	audioDevice = alcOpenDevice (NULL);
	if (!audioDevice)
		return false;

	audioContext = alcCreateContext (audioDevice, NULL);
	alcMakeContextCurrent (audioContext);
	if (!audioContext)
		return false;

	ALenum error = alutGetError ();
	if (error != AL_NO_ERROR)
		return false;
	return true;
}


void AudioSystem::destroy ()
{
	alcMakeContextCurrent (NULL);
	alcDestroyContext (audioContext);
	alcCloseDevice (audioDevice);
	alutExit ();
}

}	// namespace Engine

