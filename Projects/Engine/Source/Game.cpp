#include "Game.h"

#include "Ticker.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "InputHandler.h"
#include "AudioSystem.h"
#include "XMLParser.h"
#include "ObjectManager.h"


namespace Engine
{

Game* Game::s_pInstance = nullptr;


Game::Game (const char* title)
	: m_state (State::UnInited),
	m_pRenderSystem (new RenderSystem (title)),
	m_pPhysicsSystem (new PhysicsSystem),
	m_pAudioSystem (new AudioSystem),
	m_pTimer (new Ticker)
{

}


Game& Game::GetInstance ()
{
	if (s_pInstance == nullptr)
		s_pInstance = new Game ("Project lab Gurzo Lajos");
	return *s_pInstance;
}


void Game::DeleteInstance ()
{
	if (s_pInstance)
	{
		delete s_pInstance;
		s_pInstance = nullptr;
		ObjectManager::GetSingletonInstance ().DeleteSingletonInstance ();
		InputHandler::DeleteInstance ();
	}
}


bool Game::IsExist ()
{
	return (s_pInstance != nullptr);
}


bool Game::Init ()
{
	if (!m_pRenderSystem->init ())
		return false;
	if (!InputHandler::GetInstance ().init ())
		return false;
	if (!m_pPhysicsSystem->init ())
		return false;
	if (!m_pAudioSystem->init ())
		return false;
	if (!XMLParser::GetInstance ().Init ())
		return false;

	m_state = State::Inited;

	return true;
}


void Game::Start ()
{
	if (m_state == State::Running)
		return;

	if (m_state == State::UnInited)
	{
		if (!Init ())
		{
			Destroy ();
			return;
		}
		else
			m_pTimer->Pause ();
	}
	
	ObjectManager::GetSingletonInstance ().Start ();
	
	m_state = State::Running;
	
	MainLoop ();
}


void Game::Pause ()
{
	switch (m_state)
	{
		case State::Inited:
			Start ();
			break;

		case State::Running:
			m_state = State::Stopped;
			break;
	}
}


void Game::MainLoop ()
{
	while (m_pTimer)
	{
		if (m_state == State::Running)
		{
			m_pTimer->Tick ();
			float dt = m_pTimer->GetTimeFromLastFrame ();
			float t = m_pTimer->GetTimeFromStart ();
			if (!Update (t, dt))
				Destroy ();
		}
	}
}


bool Game::Update (float t, float dt)
{
	if (!InputHandler::GetInstance ().update (t, dt))
		return false;

	ObjectManager::GetSingletonInstance ().PreUpdate (t, dt); // fizika elotti teendok befrissitese

	if (!m_pPhysicsSystem->update (t, dt)) // fizikai rendszer befrissitese
		return false;

	ObjectManager::GetSingletonInstance ().Update (t, dt); // fizika befrissitese

	ObjectManager::GetSingletonInstance ().PostUpdate (t, dt); // fizika utani teendok elvegzese

	if (!m_pAudioSystem->update (t, dt))
		return false;

	if (!m_pRenderSystem->update (t, dt)) // kirajzolas
		return false;

	return true;
}


void Game::Destroy ()
{
	if (m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = nullptr;
	}
	else return;

	XMLParser::GetInstance ().Destroy ();
	ObjectManager::GetSingletonInstance ().Destroy ();
	InputHandler::GetInstance ().destroy ();
	m_pPhysicsSystem->destroy ();
	m_pRenderSystem->destroy ();
	m_pAudioSystem->destroy ();
}

}	// namespace Engine
