#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include "SingletonBase.h"
#include <map>
#include <AL/alut.h>


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
	bool								GetAvailableSource (unsigned int* outSrcID) const;
	bool								IsPlaying (unsigned int sourceID) const;

	DLL_EXPORT void						SetPathToBuffers (const std::string& pathToBuffers);
	DLL_EXPORT void						SetListener (const std::string& listenerName);
	
private:
	friend class SingletonBase<AudioManager>;

	static constexpr unsigned char			s_MaxSourceCount = 16;

	ALCdevice*								m_pAudioDevice;
	ALCcontext*								m_pAudioContext;

	unsigned int							m_sourceIDs[s_MaxSourceCount];
	std::map<std::string, unsigned int>		m_bufferIDs;

	std::string								m_pathToBuffers;

	std::shared_ptr<GameObject>				m_pListenerObj;


										AudioManager ();
										~AudioManager () = default;
};

}	// namespace Engine

#endif	// #ifndef AUDIO_MANAGER_H
