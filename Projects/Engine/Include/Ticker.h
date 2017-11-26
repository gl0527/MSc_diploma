#ifndef TICKER_H
#define TICKER_H

#pragma once

namespace Engine {

// ================================ class Ticker ================================

class Ticker
{
public:
	Ticker ();

	void	Tick ();
	void	Pause ();
	float	GetTimeFromStart ()	const { return m_timeFromStart; }
	float	GetTimeFromLastFrame ()	const { return m_timeFromLastFrame; }

private:
	unsigned int	m_lastTimeFromStart;
	float			m_timeFromStart;
	float			m_timeFromLastFrame;
};

}	// namespace Engine

#endif	// #ifndef TICKER_H
