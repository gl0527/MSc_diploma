#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include "SingletonBase.h"
#include <map>
#include <AL/alut.h>
#include "ErrorMessage.h"


#define AL_SAFE_CALL(fn, msg) WRAP ( \
	fn; \
	ALenum error = alGetError (); \
	if (error != AL_NO_ERROR) \
		ERR_LOG (std::cerr, std::string (alutGetErrorString (error)) + ": " + msg); \
); 

#define ALUT_SAFE_CALL(fn, msg) WRAP ( \
	fn; \
	ALenum error = alutGetError (); \
	if (error != ALUT_ERROR_NO_ERROR) \
		ERR_LOG (std::cerr, std::string (alutGetErrorString(error)) + ": " + msg); \
);


namespace Engine {

class GameObject;

// ============================= class AudioManager =============================

class AudioManager : public SingletonBase<AudioManager>
{
public:
	static DLL_EXPORT AudioManager&		GetInstance ();

	void								Init ();
	void								Update ();
	void								Destroy ();

	void								GetBuffer (const std::string& bufferName, unsigned int* outBufferID);
	bool								GetAvailableSource (unsigned int* outSrcID);
	bool								IsInitialized () const;
	bool								IsEnabled () const;

	bool								IsPlaying (unsigned int sourceID) const;
	bool								IsPaused (unsigned int sourceID) const;
	bool								IsStopped (unsigned int sourceID) const;

	void								Enable ();
	void								Disable ();
	
	void								UnleashSource (unsigned int sourceID);
	DLL_EXPORT void						SetPathToBuffers (const std::string& pathToBuffers);
	DLL_EXPORT void						SetListener (const std::string& listenerName);
	
private:
	friend class SingletonBase<AudioManager>;

	static constexpr unsigned char			s_MaxSourceCount = 16;

	ALCdevice*								m_pAudioDevice;
	ALCcontext*								m_pAudioContext;

	unsigned int							m_sourceIDs[s_MaxSourceCount];	// nem kene ennyi limitacio
	bool									m_sourcesInUse[s_MaxSourceCount];
	std::map<std::string, unsigned int>		m_bufferIDs;

	std::string								m_pathToBuffers;

	std::shared_ptr<GameObject>				m_pListenerObj;

	bool									m_isInitialized;
	bool									m_isEnabled;


										AudioManager ();
										~AudioManager () = default;
};

}	// namespace Engine

#endif	// #ifndef AUDIO_MANAGER_H
