#include "Ticker.h"

#ifdef _WIN32
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

Ticker::Ticker (float limit /*= 0.0f*/) :
	m_lastFrameTimeInSec (0.0f),
	m_uptimeInSec (0.0f),
	m_lastFrameDurationInSec (0.0f),
	m_limitInSec (limit)
{
}


void Ticker::Tick ()
{
	float currFrameTimeInSec = ::timeGetTime () * 0.001f;

	if (m_lastFrameTimeInSec < 1.0e-4)
		m_lastFrameTimeInSec = currFrameTimeInSec;

	m_lastFrameDurationInSec = currFrameTimeInSec - m_lastFrameTimeInSec;
	m_uptimeInSec += m_lastFrameDurationInSec;

	if (m_limitInSec > 0.0f && m_uptimeInSec > m_limitInSec)
		Reset ();

	m_lastFrameTimeInSec = currFrameTimeInSec;
}


void Ticker::Reset ()
{
	m_lastFrameTimeInSec = 0.0f;
	m_uptimeInSec = 0.0f;
	m_lastFrameDurationInSec = 0.0f;
}


void Ticker::Pause ()
{
	m_lastFrameTimeInSec = ::timeGetTime () * 0.001f;
}


void Ticker::UptimeInSec (float* pOutSec) const
{
	*pOutSec = m_uptimeInSec;
}


void Ticker::UptimeInHourMinSec (unsigned long* pOutHour, unsigned long* pOutMin, unsigned long* pOutSec) const
{
	*pOutHour = static_cast<unsigned long> (m_uptimeInSec) / 3600;
	*pOutMin = (static_cast<unsigned long> (m_uptimeInSec) / 60) % 60;
	*pOutSec = static_cast<unsigned long> (m_uptimeInSec) % 60;
}


void Ticker::LastFrameDurationInSec (float* pOutSec) const
{
	*pOutSec = m_lastFrameDurationInSec;
}

}	// namespace Engine
