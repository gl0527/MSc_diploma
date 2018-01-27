#include "AudioComponent.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include <iostream>
#include "Ogre.h"
#include "TransformComponent.h"


namespace Engine {

AudioComponent::Descriptor::Descriptor ()
	: fileName (""),
	listenerName (""),
	volume (0.0f),
	speed (0.0f),
	loop (false)
{
}


AudioComponent::AudioComponent (const std::string& fileName, const std::string& listenerName)
	: Component (fileName),
	m_volume (0.0f),
	m_speed (0.0f),
	m_looping (false)
{
	m_buffer = alutCreateBufferFromFile (fileName.c_str ());
	ALenum error = alutGetError ();
	if (error != AL_NO_ERROR)
		std::cout << "Cannot load audio file.\n";
	alGenSources (1, &m_source);
	alSourcei (m_source, AL_BUFFER, m_buffer);
	m_pListener = ObjectManager::GetInstance ().GetGameObjectByName (listenerName);
}


AudioComponent::AudioComponent (const Descriptor& desc)
	: AudioComponent (desc.fileName, desc.listenerName)
{
}


AudioComponent::~AudioComponent ()
{
	alDeleteSources (1, &m_source);
	alDeleteBuffers (1, &m_buffer);
}


void AudioComponent::UpdatePose (const Ogre::Vector3& pos, const Ogre::Vector3& dir)
{
	alSource3f (m_source, AL_POSITION, pos.x, pos.y, pos.z);
	alSource3f (m_source, AL_VELOCITY, dir.x, dir.y, dir.z);
	alSource3f (m_source, AL_DIRECTION, dir.x, dir.y, dir.z);
}


void AudioComponent::PreUpdate (float t, float dt)
{
	if (m_owner)
		UpdatePose (m_owner->Transform ()->GetPositionInWorldSpace (), m_owner->Transform ()->GetForwardVecInWorldSpace ());
	if (auto listenerObj = m_pListener.lock ())
		UpdatePose (listenerObj->Transform ()->GetPositionInWorldSpace (), listenerObj->Transform ()->GetForwardVecInWorldSpace ());
}


void AudioComponent::Play ()
{
	if (IsPlaying ())
		return;
	if (m_looping)
		alSourcei (m_source, AL_LOOPING, AL_TRUE);
	alSourcef (m_source, AL_GAIN, m_volume);
	alSourcef (m_source, AL_PITCH, m_speed);
	alSourcePlay (m_source);
}


void AudioComponent::Pause ()
{
	alGetError ();
	alSourcePause (m_source);
}


void AudioComponent::Stop ()
{
	alGetError ();
	alSourceStop (m_source);
}


bool AudioComponent::IsPlaying ()
{
	ALenum state;
	alGetSourcei (m_source, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}


void AudioComponent::SetVolume (float vol)
{
	m_volume = vol;
	alGetError ();
	alSourcef (m_source, AL_GAIN, m_volume);
	if (alGetError () != AL_NO_ERROR)
		std::cout << "AudioComponent::setVolume: Error occured during setting volume.\n";
}


void AudioComponent::SetSpeed (float spd)
{
	m_speed = spd;
	alGetError ();
	alSourcef (m_source, AL_PITCH, m_speed);
	if (alGetError () != AL_NO_ERROR)
		std::cout << "AudioComponent::setVolume: Error occured during setting speed.\n";
}


void AudioComponent::SetLooping (bool loop)
{
	m_looping = loop;
	alGetError ();
	if (m_looping)
		alSourcei (m_source, AL_LOOPING, AL_TRUE);
	else
		alSourcei (m_source, AL_LOOPING, AL_FALSE);
	if (alGetError () != AL_NO_ERROR)
		std::cout << "AudioComponent::setVolume: Error occured during setting looping.\n";
}


void AudioComponent::ApplyDescriptor (const Descriptor& desc)
{
	SetVolume (desc.volume);
	SetSpeed (desc.speed);
	SetLooping (desc.loop);
}

}	// namespace Engine
