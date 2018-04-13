#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "System.h"
#include "OgrePrerequisites.h"
#include <OIS.h>
#include "Overlay\OgreOverlayManager.h"
#include "MyGUI_Gui.h"
#include "stdafx.h"


namespace Ogre {

class OverlaySystem;
class Root;
class SceneNode;
class SceneManager;
class RenderWindow;
class OverlayElement;
class Overlay;
class OverlayContainer;
class OverlayManager;
class Overlay;

}

namespace MyGUI {

class OgrePlatform;

}

namespace Engine {

// ============================= class RenderSystem =============================

class RenderSystem :	public System,
						public OIS::MouseListener,
						public OIS::KeyListener
{
public:
									RenderSystem (const char* wName, size_t w = 800, size_t h = 600);

	virtual bool					init () override;
	bool							Start ();
	virtual bool					update (float t, float dt) override;
	virtual void					destroy () override;

	// inherited from OIS::MouseListener
	bool							mouseMoved (const OIS::MouseEvent& me) override;
	bool							mousePressed (const OIS::MouseEvent& me, OIS::MouseButtonID id) override;
	bool							mouseReleased (const OIS::MouseEvent& me, OIS::MouseButtonID id) override;

	// inherited from OIS::KeyListener
	bool							keyPressed (const OIS::KeyEvent& ke) override;
	bool							keyReleased (const OIS::KeyEvent& ke) override;

	static DLL_EXPORT Ogre::MeshPtr	CreatePlaneMeshXZ (const char* planeMeshName, float y, unsigned int u = 1, unsigned int v = 1);
	static Ogre::TexturePtr			CreateTexture (const char* texName, unsigned int w, unsigned int h);

	void							LoadResourceGroup (const std::string& resGroupName);
	DLL_EXPORT void					UnloadResourceGroup (const std::string& resGroupName);

	Ogre::Root*						GetRoot () const;
 	DLL_EXPORT Ogre::SceneNode*		GetRootNode () const;
	DLL_EXPORT Ogre::SceneManager*	GetSceneManager () const;
	DLL_EXPORT Ogre::RenderWindow*	GetRenderWindow () const;

	Ogre::OverlayElement*			GetOverlayElement (const char* elementName) const;
	Ogre::Overlay*					GetOverlay (const char* overlayName) const;
	Ogre::OverlayContainer*			GetContainer (const char* containerName) const;
	Ogre::OverlayManager*			GetOverlayMgr () const;

	DLL_EXPORT Ogre::Overlay*		CreateOverlay (const char* name);

	template<typename T>
	T* 								CreateOverlayElement (const char* typeName, const char* name);

	DLL_EXPORT void					LoadGUILayout (const std::string& layoutFileName);

	template<typename T>
	T*								GetWidget (const std::string& widgetName);

	void							SetActiveViewport ();

private:
	Ogre::Root*				m_pOgreRoot;
	Ogre::SceneManager*		m_pSceneMgr;
	Ogre::RenderWindow*		m_pRenderWnd;
	Ogre::OverlaySystem*	m_pOverlaySys;
	Ogre::OverlayManager*	m_pOverlayMgr;
	MyGUI::OgrePlatform*	m_pOgrePlatform;
	MyGUI::Gui*				m_pGUI;

	std::string				m_wndName;
	size_t					m_wndWidth;
	size_t					m_wndHeight;
};


template<typename T>
inline T* RenderSystem::CreateOverlayElement (const char* typeName, const char* name)
{
	return reinterpret_cast<T*> (m_pOverlayMgr->createOverlayElement (typeName, name));
}


template<typename T>
inline T* RenderSystem::GetWidget (const std::string& widgetName)
{
	return m_pGUI->findWidget<T> (widgetName);
}

}	// namespace Engine

#endif	// #ifndef RENDER_SYSTEM_H
