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
	m_pRenderWnd (nullptr),
	m_pGUIAsKeyListener (nullptr),
	m_pGUIAsMouseListener (nullptr),
	m_mouseEventProcessedByGUI (MouseEventProcessedByGUI::None)
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

		RemoveAllListeners ();
    }
}


bool InputManager::AddGUIAsKeyListener (OIS::KeyListener* keyListener)
{
	if (keyListener == nullptr)
		return false;

	if (m_pGUIAsKeyListener == nullptr) {
		m_pGUIAsKeyListener = keyListener;

		return true;
	}

	return false;
}


bool InputManager::AddGUIAsMouseListener (OIS::MouseListener* mouseListener)
{
	if (mouseListener == nullptr)
		return false;

	if (m_pGUIAsMouseListener == nullptr) {
		m_pGUIAsMouseListener = mouseListener;

		return true;
	}

	return false;
}


bool InputManager::AddKeyListener (OIS::KeyListener* keyListener, const std::string& instanceName)
{
	if (keyListener == nullptr)
		return false;

	auto it = m_keyListeners.find (instanceName);
	if (it == m_keyListeners.end ()) {
		m_keyListeners[instanceName] = keyListener;

		return true;
	}

	return false;
}


bool InputManager::AddMouseListener (OIS::MouseListener* mouseListener, const std::string& instanceName)
{
	if (mouseListener == nullptr)
		return false;
	
	auto it = m_mouseListeners.find (instanceName);
	if (it == m_mouseListeners.end ()) {
		m_mouseListeners[instanceName] = mouseListener;
		
		return true;
	}

	return false;
}


bool InputManager::RemoveKeyListener (const std::string& instanceName)
{
	auto it = m_keyListeners.find (instanceName);
	if (it != m_keyListeners.end ()) {
		m_keyListeners.erase (it);

		return true;
	}

	return false;
}


bool InputManager::RemoveKeyListener (OIS::KeyListener* keyListener)
{
	if (keyListener == nullptr)
		return false;
	
	for (auto it = m_keyListeners.begin (), itEnd = m_keyListeners.end (); it != itEnd; ++it) {
		if (it->second == keyListener) {
			m_keyListeners.erase (it);
			
			return true;
		}
	}

	return false;
}


bool InputManager::RemoveMouseListener (const std::string& instanceName)
{
	auto it = m_mouseListeners.find (instanceName);
	if (it != m_mouseListeners.end ()) {
		m_mouseListeners.erase (it);

		return true;
	}

	return false;
}


bool InputManager::RemoveMouseListener (OIS::MouseListener* mouseListener)
{
	if (mouseListener == nullptr)
		return false;

	for (auto it = m_mouseListeners.begin (), itEnd = m_mouseListeners.end (); it != itEnd; ++it) {
		if (it->second == mouseListener) {
			m_mouseListeners.erase (it);
			
			return true;
		}
	}

	return false;
}


void InputManager::RemoveAllListeners ()
{
	m_keyListeners.clear ();
	m_mouseListeners.clear ();
}


void InputManager::RemoveAllKeyListeners ()
{
	m_keyListeners.clear ();
}


void InputManager::RemoveAllMouseListeners ()
{
	m_mouseListeners.clear ();
}


void InputManager::SetWindowExtents (int width, int height)
{
	const OIS::MouseState &mouseState = m_pMouse->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
}


bool InputManager::mouseMoved (const OIS::MouseEvent& me)
{
	if (m_pGUIAsMouseListener->mouseMoved (me)) {
		m_mouseEventProcessedByGUI = MouseEventProcessedByGUI::MouseMoved;

		return true;
	} else {
		m_mouseEventProcessedByGUI = MouseEventProcessedByGUI::None;
	}

	for (auto it = m_mouseListeners.begin (), itEnd = m_mouseListeners.end (); it != itEnd; ++it) {
		if (!it->second->mouseMoved (me))
			return false;
	}

	return true;
}


bool InputManager::mousePressed (const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	if (m_pGUIAsMouseListener->mousePressed (me, id)) {
		if (id == OIS::MB_Left)
			m_mouseEventProcessedByGUI = MouseEventProcessedByGUI::LeftMouseButtonPushed;
		else if (id == OIS::MB_Right)
			m_mouseEventProcessedByGUI = MouseEventProcessedByGUI::RightMouseButtonPushed;
		else if (id == OIS::MB_Middle)
			m_mouseEventProcessedByGUI = MouseEventProcessedByGUI::MiddleMouseButtonPushed;

		return true;
	} else {
		m_mouseEventProcessedByGUI = MouseEventProcessedByGUI::None;
	}

	for (auto it = m_mouseListeners.begin (), itEnd = m_mouseListeners.end (); it != itEnd; ++it) {
		if (!it->second->mousePressed (me, id))
			return false;
	}

	return true;
}


bool InputManager::mouseReleased (const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	if (m_pGUIAsMouseListener->mouseReleased (me, id))
		return true;

	for (auto it = m_mouseListeners.begin (), itEnd = m_mouseListeners.end (); it != itEnd; ++it) {
		if (!it->second->mouseReleased (me, id))
			return false;
	}

	return true;
}


bool InputManager::keyPressed (const OIS::KeyEvent& ke)
{
	if (m_pGUIAsKeyListener->keyPressed (ke)) {
		m_keyEventProcessedByGUI = KeyEventProcessedByGUI::Any;
		
		return true;
	} else {
		m_keyEventProcessedByGUI = KeyEventProcessedByGUI::None;
	}

	for (auto it = m_keyListeners.begin (), itEnd = m_keyListeners.end (); it != itEnd; ++it) {
		if (!it->second->keyPressed (ke))
			return false;
	}
	
	return true;
}


bool InputManager::keyReleased (const OIS::KeyEvent& ke)
{
	if (m_pGUIAsKeyListener->keyReleased (ke))
		return true;

	for (auto it = m_keyListeners.begin (), itEnd = m_keyListeners.end (); it != itEnd; ++it) {
		if (!it->second->keyReleased (ke))
			return false;
	}

	return true;
}


bool InputManager::IsKeyDown (OIS::KeyCode key) const
{
	if (m_keyEventProcessedByGUI == KeyEventProcessedByGUI::Any)
		return false;
	
	if (m_pKeyboard != nullptr)
		return m_pKeyboard->isKeyDown (key);
	else
		return false;
}


bool InputManager::IsLeftMouseButtonDown () const
{
	if (m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::LeftMouseButtonPushed ||
		m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::MouseMoved)
		return false;
	
	if (m_pMouse != nullptr)
		return m_pMouse->getMouseState ().buttonDown (OIS::MB_Left);
	else 
		return false;
}


bool InputManager::IsRightMouseButtonDown () const
{
	if (m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::RightMouseButtonPushed ||
		m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::MouseMoved)
		return false;

	if (m_pMouse != nullptr)
		return m_pMouse->getMouseState ().buttonDown (OIS::MB_Right);
	else
		return false;
}


bool InputManager::IsMiddleMouseButtonDown () const
{
	if (m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::MiddleMouseButtonPushed ||
		m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::MouseMoved)
		return false;

	if (m_pMouse != nullptr)
		return m_pMouse->getMouseState ().buttonDown (OIS::MB_Middle);
	else
		return false;
}


bool InputManager::GetAbsoluteMouseX (int* outAbsoluteMouseX) const
{
	if (m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::MouseMoved)
		return false;

	if (m_pMouse != nullptr) {
		*outAbsoluteMouseX = m_pMouse->getMouseState ().X.abs;

		return true;
	}

	return false;
}


bool InputManager::GetAbsoluteMouseY (int* outAbsoluteMouseY) const
{
	if (m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::MouseMoved)
		return false;

	if (m_pMouse != nullptr) {
		*outAbsoluteMouseY = m_pMouse->getMouseState ().Y.abs;

		return true;
	}

	return false;
}


bool InputManager::GetRelativeMouseX (int* outRelativeMouseX) const
{
	if (m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::MouseMoved)
		return false;
	
	if (m_pMouse != nullptr) {
		*outRelativeMouseX = m_pMouse->getMouseState ().X.rel;

		return true;
	}

	return false;
}


bool InputManager::GetRelativeMouseY (int* outRelativeMouseY) const
{
	if (m_mouseEventProcessedByGUI == MouseEventProcessedByGUI::MouseMoved)
		return false;

	if (m_pMouse != nullptr) {
		*outRelativeMouseY = m_pMouse->getMouseState ().Y.rel;

		return true;
	}

	return false;
}

}	// namespace Engine
