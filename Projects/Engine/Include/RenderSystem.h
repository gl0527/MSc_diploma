#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "SingletonBase.h"
#include "OgrePrerequisites.h"
#include <OIS.h>
#include "Overlay\OgreOverlayManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetDefines.h"
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

}	// namespace Ogre

namespace MyGUI {

class OgrePlatform;

}	// namespace MyGUI

namespace Engine {

// ============================= class RenderSystem =============================

class RenderSystem :	public SingletonBase<RenderSystem>,
						public OIS::MouseListener,
						public OIS::KeyListener
{
public:
	static DLL_EXPORT RenderSystem&		GetInstance ();

	static DLL_EXPORT Ogre::MeshPtr		CreatePlaneMeshXZ (const char* planeMeshName, float y, unsigned int u = 1, unsigned int v = 1);
	static Ogre::TexturePtr				CreateTexture (const char* texName, unsigned int w, unsigned int h);

	void								Init ();
	void								Start ();
	void								Update (float t, float dt);
	void								Destroy ();

	void								LoadResourceGroup (const std::string& resGroupName);
	DLL_EXPORT void						UnloadResourceGroup (const std::string& resGroupName);

	Ogre::Root*							GetRoot () const;
 	DLL_EXPORT Ogre::SceneNode*			GetRootNode () const;
	DLL_EXPORT Ogre::SceneManager*		GetSceneManager () const;
	DLL_EXPORT Ogre::RenderWindow*		GetRenderWindow () const;

	DLL_EXPORT float					GetCurrentFPS () const;
	DLL_EXPORT float					GetAverageFPS () const;

	DLL_EXPORT Ogre::OverlayElement*	GetOverlayElement (const char* elementName) const;
	DLL_EXPORT Ogre::Overlay*			GetOverlay (const char* overlayName) const;
	Ogre::OverlayContainer*				GetContainer (const char* containerName) const;
	Ogre::OverlayManager*				GetOverlayMgr () const;

	DLL_EXPORT Ogre::Overlay*			CreateOverlay (const char* name);

	template<typename T>
	T* 									CreateOverlayElement (const char* typeName, const char* name);

	DLL_EXPORT MyGUI::VectorWidgetPtr	LoadGUILayout (const std::string& layoutFileName);

	template<typename T>
	T*									GetWidget (const std::string& widgetName);

	void								SetActiveViewport ();

private:
	friend class SingletonBase<RenderSystem>;

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

	float					m_currentFPS;
	float					m_averageFPS;


									RenderSystem (const char* wName, size_t w = 800, size_t h = 600);

	// inherited from OIS::MouseListener
	bool							mouseMoved (const OIS::MouseEvent& me) override;
	bool							mousePressed (const OIS::MouseEvent& me, OIS::MouseButtonID id) override;
	bool							mouseReleased (const OIS::MouseEvent& me, OIS::MouseButtonID id) override;

	// inherited from OIS::KeyListener
	bool							keyPressed (const OIS::KeyEvent& ke) override;
	bool							keyReleased (const OIS::KeyEvent& ke) override;
};


template<typename T>
inline T* RenderSystem::CreateOverlayElement (const char* typeName, const char* name)
{
	return dynamic_cast<T*> (m_pOverlayMgr->createOverlayElement (typeName, name));
}


template<typename T>
inline T* RenderSystem::GetWidget (const std::string& widgetName)
{
	return m_pGUI->findWidget<T> (widgetName);
}

}	// namespace Engine

#endif	// #ifndef RENDER_SYSTEM_H
