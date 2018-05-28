#include "Game.h"

#include "Ticker.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "AudioManager.h"
#include "XML/XMLParser.h"
#include "ObjectManager.h"
#include "InputManager.h"


namespace Engine {

Game* Game::s_pInstance = nullptr;


Game::Game ()
	: m_state (State::UnInited),
	m_pTimer (new Ticker)
{
}


Game& Game::GetInstance ()
{
	if (s_pInstance == nullptr)
		s_pInstance = new Game;
	return *s_pInstance;
}


void Game::DeleteInstance ()
{
	if (s_pInstance) {
		ObjectManager::GetInstance ().DeleteInstance ();
		InputManager::GetInstance ().DeleteInstance ();
		AudioManager::GetInstance ().DeleteInstance ();
		delete s_pInstance;
		s_pInstance = nullptr;
	}
}


bool Game::IsExist ()
{
	return (s_pInstance != nullptr);
}


bool Game::Init ()
{
#ifndef __cplusplus
#error C++ compiler required!
#endif

	srand (time (nullptr));
	
	RenderSystem::GetInstance ().Init ();
	InputManager::GetInstance ().Init ();
	PhysicsSystem::GetInstance ().Init ();
	AudioManager::GetInstance ().Init ();

	if (!XML::XMLParser::GetInstance ().Init ())
		return false;

	m_state = State::Inited;

	return true;
}


void Game::Start ()
{
	if (m_state == State::Running)
		return;

	if (m_state == State::UnInited) {
		if (!Init ()) {
			Destroy ();
			return;
		}
		else
			m_pTimer->Pause ();
	}

	RenderSystem::GetInstance ().Start ();
	ObjectManager::GetInstance ().Start ();

	m_state = State::Running;
	MainLoop ();
}


void Game::Continue ()
{
	m_state = State::Running;
}

void Game::Pause ()
{
	switch (m_state) {
		case State::Inited:
			Start ();
			break;

		case State::Running:
			m_state = State::Stopped;
			break;

		default:
			break;
	}
}


void Game::MainLoop ()
{
	while (m_state != State::Destroyed) {
		m_pTimer->Tick ();

		float t = m_pTimer->GetUpTime ();
		float dt = m_pTimer->GetDeltaTime ();

		// Updating
		InputManager::GetInstance ().Capture ();

		if (m_state == State::Running) {
			AudioManager::GetInstance ().Update ();
			ObjectManager::GetInstance ().PreUpdate (t, dt);

			if (!PhysicsSystem::GetInstance ().Update (t, dt))
				Destroy ();

			ObjectManager::GetInstance ().Update (t, dt);
			ObjectManager::GetInstance ().PostUpdate (t, dt);
		}
		
		RenderSystem::GetInstance ().Update (t, dt);

		// Removing and destroying
		ObjectManager::GetInstance ().RemoveMarkedComponents ();
		ObjectManager::GetInstance ().RemoveMarkedGameObjects ();

		if (m_state == State::Destroyable)
			Destroy ();
	}
}


void Game::Destroy ()
{
	if (m_pTimer != nullptr) {
		delete m_pTimer;
		m_pTimer = nullptr;
	}
	else return;

	XML::XMLParser::GetInstance ().Destroy ();
	ObjectManager::GetInstance ().Destroy ();
	InputManager::GetInstance ().Destroy ();
	AudioManager::GetInstance ().Destroy ();
	PhysicsSystem::GetInstance ().Destroy ();
	RenderSystem::GetInstance ().Destroy ();

	m_state = State::Destroyed;
}


void Game::MarkForDestroy ()
{
	m_state = State::Destroyable;
}


Game::State Game::GetState () const
{
	return m_state;
}

}	// namespace Engine
