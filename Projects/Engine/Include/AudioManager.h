#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include "SingletonBase.h"
#include <map>
#include <vector>
#include <AL/alut.h>
#include "ErrorMessage.h"


#define AL_SAFE_CALL(fn, msg) WRAP ( \
	fn; \
	ALenum error = alGetError (); \
	if (error != AL_NO_ERROR) \
		ERR_LOG (std::cerr, \
				std::string ("Error @ ") + TOSTRING (fn) + ": " + std::string (alutGetErrorString (error))  + ": " + msg); \
);

#define ALUT_SAFE_CALL(fn, msg) WRAP ( \
	fn; \
	ALenum error = alutGetError (); \
	if (error != ALUT_ERROR_NO_ERROR) \
		ERR_LOG (std::cerr, \
				std::string ("Error @ ") + TOSTRING (fn) + ": " + std::string (alutGetErrorString(error)) + ": " + msg); \
);


namespace Engine {

class GameObject;
class AudioSourceComponent;

// ============================= class AudioManager =============================

class AudioManager : public SingletonBase<AudioManager>
{
public:
	static DLL_EXPORT AudioManager&		GetInstance ();

	void								Init ();
	void								Update ();
	void								Destroy ();

	void								AddAudioSourceComponent (const std::shared_ptr<AudioSourceComponent>& pAudioSourceComp);

	void								GetBuffer (const std::string& bufferName, unsigned int* outBufferID);
	bool								IsInitialized () const;
	DLL_EXPORT bool						IsEnabled () const;

	bool								IsPlaying (unsigned int sourceID) const;
	bool								IsPaused (unsigned int sourceID) const;
	bool								IsStopped (unsigned int sourceID) const;

	DLL_EXPORT void						Enable ();
	DLL_EXPORT void						Disable ();
	
	DLL_EXPORT void						SetResourceLocation (const std::string& pathToBuffers);
	DLL_EXPORT void						SetListener (const std::string& listenerName);
	
private:
	friend class SingletonBase<AudioManager>;

	using AudioSourceComponentVector	= std::vector<std::shared_ptr<AudioSourceComponent>>;
	using BufferMap						= std::map<std::string, unsigned int>;

	static constexpr unsigned char	s_MaxUsedSources = 16;

	ALCdevice*						m_pAudioDevice;
	ALCcontext*						m_pAudioContext;

	AudioSourceComponentVector		m_audioSourceComponents;
	unsigned int					m_sourceIDs[s_MaxUsedSources];
	BufferMap						m_bufferIDs;

	std::string						m_resourceLocation;

	std::shared_ptr<GameObject>		m_pListenerObj;

	bool							m_isInitialized;
	bool							m_isEnabled;


										AudioManager ();
										~AudioManager () = default;
};

}	// namespace Engine

#endif	// #ifndef AUDIO_MANAGER_H
