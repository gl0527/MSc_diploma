#include "InputHandler.h"
#include <sstream>
#include "Game.h"
#include "RenderSystem.h"
#include "Ogre.h"


namespace Engine {

InputHandler::InputHandler ()
	: m_pInputManager (nullptr),
	m_pKeyboard (nullptr),
	m_pMouse (nullptr),
	m_pRenderWindow (nullptr)
{
}


InputHandler& InputHandler::GetInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (new InputHandler); });
	return *s_pInstance.get ();
}


bool InputHandler::init ()
{
	OIS::ParamList pl;
	size_t windowHnd = 0;
	m_pRenderWindow = Game::GetInstance ().GetRenderSystem ()->getRenderWindow ();
	m_pRenderWindow->getCustomAttribute ("WINDOW", &windowHnd);
	std::ostringstream windowHndStr;
	windowHndStr << windowHnd;

	pl.insert (std::make_pair (std::string ("WINDOW"), windowHndStr.str ()));

#if defined (CURSOR_VISIBLE)
#if defined (OIS_WIN32_PLATFORM)
	pl.insert (std::make_pair (std::string ("w32_mouse"), std::string ("DISCL_FOREGROUND")));
	pl.insert (std::make_pair (std::string ("w32_mouse"), std::string ("DISCL_NONEXCLUSIVE")));
	pl.insert (std::make_pair (std::string ("w32_keyboard"), std::string ("DISCL_FOREGROUND")));
	pl.insert (std::make_pair (std::string ("w32_keyboard"), std::string ("DISCL_NONEXCLUSIVE")));
#elif defined (OIS_LINUX_PLATFORM)
	pl.insert (std::make_pair (std::string ("x11_mouse_grab"), std::string ("false")));
	pl.insert (std::make_pair (std::string ("x11_mouse_hide"), std::string ("false")));
	pl.insert (std::make_pair (std::string ("x11_keyboard_grab"), std::string ("false")));
	pl.insert (std::make_pair (std::string ("XAutoRepeatOn"), std::string ("true")));
#endif	// #if defined OIS_WIN32_PLATFORM
#endif	// #ifdef CURSOR_VISIBLE

	m_pInputManager = OIS::InputManager::createInputSystem (pl);
	m_pKeyboard =
		static_cast<OIS::Keyboard*> (m_pInputManager->createInputObject (OIS::OISKeyboard, false)); // unbuffered keyboard
	m_pMouse =
		static_cast<OIS::Mouse*>(m_pInputManager->createInputObject (OIS::OISMouse, false)); // unbuffered mouse

	return true;
}


bool InputHandler::update (float t, float dt)
{
	if (m_pKeyboard && m_pMouse) {
		m_pKeyboard->capture ();
		m_pMouse->capture ();
	}
	else
		return false;

	const OIS::MouseState& ms = m_pMouse->getMouseState ();
	ms.width = m_pRenderWindow->getViewport (0)->getActualWidth ();
	ms.height = m_pRenderWindow->getViewport (0)->getActualHeight ();

	return true;
}


bool InputHandler::IsButtonDown (OIS::KeyCode key) const
{
	if (m_pKeyboard)
		return m_pKeyboard->isKeyDown (key);
	else
		return false;
}


bool InputHandler::IsLeftMouseButtonDown () const
{
	if (m_pMouse)
		return m_pMouse->getMouseState ().buttonDown (OIS::MB_Left);
	else
		return false;
}


bool InputHandler::IsRightMouseButtonDown () const
{
	if (m_pMouse)
		return m_pMouse->getMouseState ().buttonDown (OIS::MB_Right);
	else
		return false;
}


bool InputHandler::IsMiddleMouseButtonDown () const
{
	if (m_pMouse)
		return m_pMouse->getMouseState ().buttonDown (OIS::MB_Middle);
	else
		return false;
}


bool InputHandler::GetAbsoluteMouseX (int* outAbsoluteMouseX) const
{
	if (m_pMouse) {
		*outAbsoluteMouseX = m_pMouse->getMouseState ().X.abs;

		return true;
	}

	return false;
}


bool InputHandler::GetAbsoluteMouseY (int* outAbsoluteMouseY) const
{
	if (m_pMouse) {
		*outAbsoluteMouseY = m_pMouse->getMouseState ().Y.abs;

		return true;
	}

	return false;
}

bool InputHandler::GetRelativeMouseX (int* outRelativeMouseX) const
{
	if (m_pMouse) {
		*outRelativeMouseX = m_pMouse->getMouseState ().X.rel;

		return true;
	}

	return false;
}


bool InputHandler::GetRelativeMouseY (int* outRelativeMouseY) const
{
	if (m_pMouse) {
		*outRelativeMouseY = m_pMouse->getMouseState ().Y.rel;;

		return true;
	}

	return false;
}


void InputHandler::destroy ()
{
	if (m_pInputManager) {
		if (m_pMouse) {
			m_pInputManager->destroyInputObject (m_pMouse);
			m_pMouse = nullptr;
		}
		if (m_pKeyboard) {
			m_pInputManager->destroyInputObject (m_pKeyboard);
			m_pKeyboard = nullptr;
		}
		m_pInputManager->destroyInputSystem (m_pInputManager);
		m_pInputManager = nullptr;
	}
}

}	// namespace Engine
