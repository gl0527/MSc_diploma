#ifndef TICKER_H
#define TICKER_H

#pragma once

#include "stdafx.h"

namespace Engine {

// ================================ class Ticker ================================

class Ticker
{
public:
						Ticker (float limit = 0.0f);

	void				Tick ();
	void				Reset ();
	DLL_EXPORT void		Pause ();

	DLL_EXPORT void		UptimeInSec (float* pOutSec) const;
	void				UptimeInHourMinSec (unsigned long* pOutHour, unsigned long* pOutMin, unsigned long* pOutSec) const;
	DLL_EXPORT void		LastFrameDurationInSec (float* pOutSec) const;

private:
	float m_lastFrameTimeInSec;
	float m_uptimeInSec;
	float m_lastFrameDurationInSec;
	float m_limitInSec;
};

}	// namespace Engine

#endif	// #ifndef TICKER_H
