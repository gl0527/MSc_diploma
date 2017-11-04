#include "Ticker.h"

#ifdef _WIN32
#include <windows.h>
#else
#error Platform not supported!
#endif


namespace Engine
{

Ticker::Ticker () :
	m_lastTimeFromStart (0),
	m_timeFromStart (0.0f),
	m_timeFromLastFrame (0.0f)
{
}


void Ticker::Tick ()
{
	unsigned int currentTime = ::timeGetTime ();

	if (m_lastTimeFromStart == 0)
		m_lastTimeFromStart = currentTime;

	m_timeFromLastFrame = (currentTime - m_lastTimeFromStart) * 0.001f;
	m_timeFromStart += m_timeFromLastFrame;
	m_lastTimeFromStart = currentTime;
}


void Ticker::Pause ()
{
	m_lastTimeFromStart = ::timeGetTime ();
}

}	// namespace Engine
