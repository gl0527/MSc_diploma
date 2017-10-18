#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "SingletonBase.h"
#include <OIS\OIS.h>
#include "System.h"
#include "stdafx.h"


// ----------------------------- forward declaration-----------------------------

namespace Ogre
{
class RenderWindow;
}


namespace Engine
{

// ============================= class InputHandler =============================

class DLL_SPEC InputHandler final : public System, public SingletonBase<InputHandler>
{
public:
	bool init () override;
	bool update (float t, float dt) override;
	void destroy () override;

	static InputHandler& GetInstance ();

	bool IsButtonDown (OIS::KeyCode key) const;
	bool IsLeftMouseButtonDown () const;
	bool IsRightMouseButtonDown () const;
	bool IsMiddleMouseButtonDown () const;

	int GetAbsoluteMouseX () const;
	int GetAbsoluteMouseY () const;
	int GetRelativeMouseX () const;
	int GetRelativeMouseY () const;

private:
	friend class SingletonBase<InputHandler>;

	OIS::InputManager*	m_pInputManager;
	OIS::Keyboard*		m_pKeyboard;
	OIS::Mouse*			m_pMouse;
	Ogre::RenderWindow*	m_pRenderWindow;

	InputHandler ();
	~InputHandler () = default;
};

}	// namespace Engine

#endif	// #ifndef INPUT_HANDLER_H
