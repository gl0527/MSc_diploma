#ifndef GAME_H
#define GAME_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"


namespace Engine
{

// ----------------------------- forward declaration -----------------------------

class RenderSystem;
class PhysicsSystem;
class AudioSystem;
class Ticker;

// ================================== class Game ==================================

class DLL_EXPORT Game
{
public:
	static Game&	GetInstance ();
	static void		DeleteInstance ();
	static bool		IsExist ();

	bool	Init ();
	void	Start ();
	void	Pause ();
	void	Destroy ();

	// TODO ezeket nem lenne szabad hasznalni, helyettuk wrapper fuggvenyek
	RenderSystem*	GetRenderSystem ()	const { return m_pRenderSystem; }
	PhysicsSystem*	getPhysicsSystem () const { return m_pPhysicsSystem; }
	
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

	RenderSystem*	m_pRenderSystem;
	PhysicsSystem*	m_pPhysicsSystem;
	AudioSystem*	m_pAudioSystem;
	Ticker*			m_pTimer;


	Game (const char* title);
	
	void MainLoop ();
	bool Update (float t, float dt);
};

}	// namespace Engine

#endif	// #ifndef GAME_H
