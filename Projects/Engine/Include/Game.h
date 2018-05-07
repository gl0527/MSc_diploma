#ifndef GAME_H
#define GAME_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"


namespace Engine {

// ----------------------------- forward declaration -----------------------------

class AudioSystem;
class Ticker;

// ================================== class Game ==================================

class Game
{
public:
	static DLL_EXPORT Game&		GetInstance ();
	static DLL_EXPORT void		DeleteInstance ();
	static bool					IsExist ();

	DLL_EXPORT bool				Init ();
	DLL_EXPORT void				Start ();
	void						Pause ();
	DLL_EXPORT void				Destroy ();

private:
	enum class State : unsigned char
	{
		UnInited,
		Inited,
		Running,
		Stopped,
		Destroyed
	};

	static Game*	s_pInstance;

	volatile State	m_state;
	Ticker*			m_pTimer;


								Game (const char* title);

	void						MainLoop ();
	bool						Update (float t, float dt);
};

}	// namespace Engine

#endif	// #ifndef GAME_H
