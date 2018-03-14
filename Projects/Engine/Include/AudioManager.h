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
	static unsigned char				GetMaxSourceCount ();

	void								Init ();
	void								Update ();
	void								Destroy ();

	DLL_EXPORT void						AddBuffer (const std::string& filePath);
	bool								GetBuffer (const std::string& bufferName, ALuint* outBuffer);

	DLL_EXPORT void						SetListener (const std::string& listenerName);
	
private:
	friend class SingletonBase<AudioManager>;

	static constexpr unsigned char	s_MaxSourceCount = 8;

	ALCdevice*						m_pAudioDevice;
	ALCcontext*						m_pAudioContext;
		
	std::map<std::string, ALuint>	m_buffers;	// sound datas, could be any of them

	std::shared_ptr<GameObject>		m_pListenerObj;	// it is the camera for most cases

										AudioManager ();
										~AudioManager () = default;
};

}	// namespace Engine

#endif	// #ifndef AUDIO_MANAGER_H
