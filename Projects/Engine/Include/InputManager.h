#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"

#include <map>
#include <string.h>
#include <OIS.h>

#include "SingletonBase.h"


namespace Ogre {

class RenderWindow;

}

namespace Engine {	

// ============================= class InputManager ==============================

class InputManager : 	public OIS::MouseListener,
						public OIS::KeyListener,
						public SingletonBase<InputManager>
{	
public:
	static DLL_EXPORT InputManager&	GetInstance ();
	
	void							Init ();
	void							Capture ();
	void 							Destroy ();
	
	bool							AddGUIAsKeyListener (OIS::KeyListener* keyListener);
	bool							AddGUIAsMouseListener (OIS::MouseListener* mouseListener);
	bool 							AddKeyListener (OIS::KeyListener* keyListener, const std::string& instanceName);
    bool 							AddMouseListener (OIS::MouseListener* mouseListener, const std::string& instanceName);
	
	bool 							RemoveKeyListener (const std::string& instanceName);
	bool 							RemoveKeyListener (OIS::KeyListener* keyListener);
    bool 							RemoveMouseListener (const std::string& instanceName);
    bool 							RemoveMouseListener (OIS::MouseListener* mouseListener);
	void 							RemoveAllListeners ();
    void 							RemoveAllKeyListeners ();
    void 							RemoveAllMouseListeners ();

	DLL_EXPORT bool					IsKeyDown (OIS::KeyCode key) const;
	DLL_EXPORT bool					IsLeftMouseButtonDown () const;
	bool							IsRightMouseButtonDown () const;
	bool							IsMiddleMouseButtonDown () const;

	DLL_EXPORT bool					GetAbsoluteMouseX (int* outAbsoluteMouseX) const;
	DLL_EXPORT bool					GetAbsoluteMouseY (int* outAbsoluteMouseY) const;
	DLL_EXPORT bool					GetRelativeMouseX (int* outRelativeMouseX) const;
	DLL_EXPORT bool					GetRelativeMouseY (int* outRelativeMouseY) const;
	
	void 							SetWindowExtents (int width, int height);
	
private:
	friend class SingletonBase<InputManager>;
	
	using KeyListenerMap	= std::map<std::string, OIS::KeyListener*>;
	using MouseListenerMap	= std::map<std::string, OIS::MouseListener*>;
	
	OIS::Keyboard*					m_pKeyboard;
	OIS::Mouse*						m_pMouse;
	OIS::InputManager*				m_pInputSystem;
	Ogre::RenderWindow*				m_pRenderWnd;
	
	OIS::KeyListener*				m_pGUIAsKeyListener;
	OIS::MouseListener*				m_pGUIAsMouseListener;
	KeyListenerMap 					m_keyListeners;
    MouseListenerMap 				m_mouseListeners;


									InputManager ();
									~InputManager () = default;

	// inherited from OIS::MouseListener
	bool							mouseMoved (const OIS::MouseEvent& me) override;
	bool							mousePressed (const OIS::MouseEvent& me, OIS::MouseButtonID id) override;
	bool							mouseReleased (const OIS::MouseEvent& me, OIS::MouseButtonID id) override;
	
	// inherited from OIS::KeyListener
	bool							keyPressed (const OIS::KeyEvent& ke) override;
	bool							keyReleased (const OIS::KeyEvent& ke) override;
};

}	// namespace Engine

#endif // #ifndef INPUT_MANAGER_H
