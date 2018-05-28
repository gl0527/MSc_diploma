#include "Ticker.h"

#ifdef WINDOWS
#include <windows.h>
#elif __linux__
#include <sys/time.h>
#endif


#ifdef __linux__
namespace {

unsigned int timeGetTime ()
{
	static struct timeval start;
	static bool first = true;
	if (first) {
		gettimeofday (&start, NULL);
		first = false;
	}
	struct timeval now;
	gettimeofday (&now, NULL);
	return now.tv_usec / 1000 + (now.tv_sec - start.tv_sec) * 1000;
}

}
#endif


namespace Engine {

Ticker::Ticker () :
	m_prevTime (0.0f),
	m_upTime (0.0f),
	m_deltaTime (0.0f)
{
}


void Ticker::Tick ()
{
	float currTime = ::timeGetTime () * 0.001f;

	if (m_prevTime < 1.0e-4)
		m_prevTime = currTime;

	m_deltaTime = currTime - m_prevTime;
	m_upTime += m_deltaTime;
	m_prevTime = currTime;
}


void Ticker::Reset ()
{
	m_prevTime = 0.0f;
	m_upTime = 0.0f;
	m_deltaTime = 0.0f;
}


void Ticker::Pause ()
{
	m_prevTime = ::timeGetTime () * 0.001f;
}


float Ticker::GetUpTime () const
{
	return m_upTime;
}


float Ticker::GetDeltaTime () const
{
	return m_deltaTime;
}

}	// namespace Engine
