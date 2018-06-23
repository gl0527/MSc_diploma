#pragma once

#ifndef TICKER_H
#define TICKER_H

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include "NonCopyable.h"

namespace Engine {

// ================================ class Ticker ================================

class Ticker : public NonCopyable
{
public:
						Ticker ();

	void				Tick ();
	void				Reset ();
	DLL_EXPORT void		Pause ();

	DLL_EXPORT float	GetUpTime () const;
	DLL_EXPORT float	GetDeltaTime () const;

private:
	float m_prevTime;
	float m_upTime;
	float m_deltaTime;
};

}	// namespace Engine

#endif	// #ifndef TICKER_H
