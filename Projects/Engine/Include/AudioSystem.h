#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "System.h"
#include "AL\alut.h"


namespace Engine {

// ============================ class AudioProcessor ============================

class AudioSystem : public System
{
public:
						AudioSystem ();
						~AudioSystem ();

	virtual bool		init () override;
	virtual void		destroy () override;

	bool				IsEnabled () const;
	void				Enable ();
	void				Disable ();

private:
	ALCdevice*			m_pAudioDevice;
	ALCcontext*			m_pAudioContext;
	bool				m_isEnabled;
};

}	// namespace Engine

#endif	// #ifndef AUDIO_SYSTEM_H
