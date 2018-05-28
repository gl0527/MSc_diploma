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
	enum class State : unsigned char
	{
		UnInited,
		Inited,
		Running,
		Stopped,
		Destroyable,
		Destroyed
	};

	static DLL_EXPORT Game&		GetInstance ();
	static DLL_EXPORT void		DeleteInstance ();
	static bool					IsExist ();

	DLL_EXPORT void				MarkForDestroy ();

	DLL_EXPORT bool				Init ();
	DLL_EXPORT void				Start ();
	DLL_EXPORT void				Continue ();
	DLL_EXPORT void				Pause ();

	DLL_EXPORT State			GetState () const;

private:
	static Game*	s_pInstance;

	volatile State	m_state;
	Ticker*			m_pTimer;


								Game ();

	void						MainLoop ();
	void						Destroy ();
};

}	// namespace Engine

#endif	// #ifndef GAME_H
