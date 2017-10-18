#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "System.h"
#include "AL\alut.h"


namespace Engine
{

// ============================ class AudioProcessor ============================

class AudioSystem : public System
{
public:
	AudioSystem ();
	~AudioSystem ();

	virtual bool init () override;
	virtual void destroy () override;

	bool isEnabled () const { return enabled; }
	void enable () { enabled = true; }
	void disable () { enabled = false; }

	const unsigned char getMaxBuffers () const { return MAX_BUFFERS; }
	const unsigned char getMaxSources () const { return MAX_SOURCES; }

private:
	ALCdevice* audioDevice;
	ALCcontext* audioContext;
	bool enabled;
	const unsigned char MAX_BUFFERS;
	const unsigned char MAX_SOURCES;
};

}	// namespace Engine

#endif	// #ifndef AUDIO_SYSTEM_H
