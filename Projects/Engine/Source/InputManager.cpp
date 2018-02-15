#include "InputManager.h"

#include <sstream>
#include "Ogre.h"
#include "Game.h"
#include "RenderSystem.h"

namespace Engine {
	
InputManager::InputManager ()
	: m_pKeyboard (nullptr),
	m_pMouse (nullptr),
	m_pInputSystem (nullptr),
	m_pRenderWnd (nullptr)
{
}


InputManager& InputManager::GetInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (new InputManager); });
	return *s_pInstance.get ();
}


void InputManager::Init ()
{
	if (m_pInputSystem == nullptr) {
        OIS::ParamList paramList;    
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;

		m_pRenderWnd = Game::GetInstance ().GetRenderSystem ()->GetRenderWindow ();
		m_pRenderWnd->getCustomAttribute ("WINDOW", &windowHnd);

        windowHndStr << (unsigned int) windowHnd;
        paramList.insert (std::make_pair (std::string ("WINDOW"), windowHndStr.str ()));

        m_pInputSystem = OIS::InputManager::createInputSystem (paramList);

        if (m_pInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
            m_pKeyboard = static_cast<OIS::Keyboard*> (m_pInputSystem->createInputObject (OIS::OISKeyboard, true));
            m_pKeyboard->setEventCallback (this);
        }
		
        if (m_pInputSystem->getNumberOfDevices (OIS::OISMouse) > 0) {
            m_pMouse = static_cast<OIS::Mouse*> (m_pInputSystem->createInputObject (OIS::OISMouse, true));
            m_pMouse->setEventCallback (this);

            unsigned int width, height, depth;
            int left, top;
			m_pRenderWnd->getMetrics (width, height, depth, left, top);

            SetWindowExtents (width, height);
		}
    }
}


void InputManager::Capture ()
{
	if (m_pMouse != nullptr)
		m_pMouse->capture ();
	
	if (m_pKeyboard != nullptr)
		m_pKeyboard->capture ();
}


void InputManager::Destroy ()
{
	if (m_pInputSystem != nullptr) {
        if (m_pMouse != nullptr) {
            m_pInputSystem->destroyInputObject (m_pMouse);
            m_pMouse = nullptr;
        }
        if (m_pKeyboard != nullptr) {
            m_pInputSystem->destroyInputObject (m_pKeyboard);
            m_pKeyboard = nullptr;
        }

		m_pInputSystem->destroyInputSystem (m_pInputSystem);
		m_pInputSystem = nullptr;

		m_keyListeners.clear();
		m_mouseListeners.clear();
    }
}


bool InputManager::AddKeyListener (OIS::KeyListener* keyListener, const std::string& instanceName)
{
	auto it = m_keyListeners.find (instanceName);
	if (it == m_keyListeners.end ()) {
		m_keyListeners[instanceName] = keyListener;

		return true;
	}

	return false;
}


bool InputManager::AddMouseListener (OIS::MouseListener* mouseListener, const std::string& instanceName)
{
	auto it = m_mouseListeners.find (instanceName);
	if (it == m_mouseListeners.end ()) {
		m_mouseListeners[instanceName] = mouseListener;
		
		return true;
	}

	return false;
}


void InputManager::SetWindowExtents (int width, int height)
{
	const OIS::MouseState &mouseState = m_pMouse->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
}


bool InputManager::mouseMoved (const OIS::MouseEvent& me)
{
	// TODO eloszor a gui-nak kell odaadni az esemenyt, aztan ha az nem kezelte le, csak akkor
	// kell odaadni a tobbi listenernek!!!
	for (auto it = m_mouseListeners.begin (), itEnd = m_mouseListeners.end (); it != itEnd; ++it) {
		if (!it->second->mouseMoved (me))
			return false;
	}

	return true;
}


bool InputManager::mousePressed (const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	for (auto it = m_mouseListeners.begin (), itEnd = m_mouseListeners.end (); it != itEnd; ++it) {
		if (!it->second->mousePressed (me, id))
			return false;
	}

	return true;
}


bool InputManager::mouseReleased (const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	for (auto it = m_mouseListeners.begin (), itEnd = m_mouseListeners.end (); it != itEnd; ++it) {
		if (!it->second->mouseReleased (me, id))
			return false;
	}

	return true;
}


bool InputManager::keyPressed (const OIS::KeyEvent& ke)
{
	for (auto it = m_keyListeners.begin (), itEnd = m_keyListeners.end (); it != itEnd; ++it) {
		if (!it->second->keyPressed (ke))
			return false;
	}
	
	return true;
}


bool InputManager::keyReleased (const OIS::KeyEvent& ke)
{
	for (auto it = m_keyListeners.begin (), itEnd = m_keyListeners.end (); it != itEnd; ++it) {
		if (!it->second->keyReleased (ke))
			return false;
	}

	return true;
}


bool InputManager::IsButtonDown (OIS::KeyCode key) const
{
	if (m_pKeyboard != nullptr)
		return m_pKeyboard->isKeyDown (key);
	else
		return false;
}


bool InputManager::IsLeftMouseButtonDown () const
{
	if (m_pMouse != nullptr)
		return m_pMouse->getMouseState ().buttonDown (OIS::MB_Left);
	else 
		return false;
}


bool InputManager::IsRightMouseButtonDown () const
{
	if (m_pMouse != nullptr)
		return m_pMouse->getMouseState ().buttonDown (OIS::MB_Right);
	else
		return false;
}


bool InputManager::IsMiddleMouseButtonDown () const
{
	if (m_pMouse != nullptr)
		return m_pMouse->getMouseState ().buttonDown (OIS::MB_Middle);
	else
		return false;
}


bool InputManager::GetAbsoluteMouseX (int* outAbsoluteMouseX) const
{
	if (m_pMouse != nullptr) {
		*outAbsoluteMouseX = m_pMouse->getMouseState ().X.abs;

		return true;
	}

	return false;
}


bool InputManager::GetAbsoluteMouseY (int* outAbsoluteMouseY) const
{
	if (m_pMouse != nullptr) {
		*outAbsoluteMouseY = m_pMouse->getMouseState ().Y.abs;

		return true;
	}

	return false;
}


bool InputManager::GetRelativeMouseX (int* outRelativeMouseX) const
{
	if (m_pMouse != nullptr) {
		*outRelativeMouseX = m_pMouse->getMouseState ().X.rel;

		return true;
	}

	return false;
}


bool InputManager::GetRelativeMouseY (int* outRelativeMouseY) const
{
	if (m_pMouse != nullptr) {
		*outRelativeMouseY = m_pMouse->getMouseState ().Y.rel;

		return true;
	}

	return false;
}

}	// namespace Engine
