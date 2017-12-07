#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "SingletonBase.h"
#include <OIS.h>
#include "stdafx.h"


// ----------------------------- forward declaration ----------------------------

namespace Ogre {
class RenderWindow;
}


namespace Engine {

// ============================= class InputHandler =============================

class InputHandler final : public SingletonBase<InputHandler>
{
public:
	bool init ();
	bool update (float t, float dt);
	void destroy ();

	static DLL_EXPORT InputHandler& GetInstance ();

	DLL_EXPORT bool IsButtonDown (OIS::KeyCode key) const;
	DLL_EXPORT bool IsLeftMouseButtonDown () const;
	bool IsRightMouseButtonDown () const;
	bool IsMiddleMouseButtonDown () const;

	bool GetAbsoluteMouseX (int* outAbsoluteMouseX) const;
	bool GetAbsoluteMouseY (int* outAbsoluteMouseY) const;
	DLL_EXPORT bool GetRelativeMouseX (int* outRelativeMouseX) const;
	DLL_EXPORT bool GetRelativeMouseY (int* outRelativeMouseY) const;

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
