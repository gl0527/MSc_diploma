#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "System.h"
#include "Ogre.h"
#include "OGRE\Overlay\OgreOverlayManager.h"
#include "OGRE\Overlay\OgreOverlaySystem.h"
#include "stdafx.h"


namespace Engine
{

// ============================= class RenderSystem =============================

class DLL_SPEC RenderSystem : public System
{
public:
	RenderSystem (const char* wName, size_t w = 800, size_t h = 600);

	virtual bool init () override;
	virtual bool update (float t, float dt) override;
	virtual void destroy () override;

	static Ogre::MeshPtr	createPlaneMeshXZ (const char* planeMeshName, float y, unsigned int u = 1, unsigned int v = 1);
	static Ogre::TexturePtr createTexture (const char* texName, unsigned int w, unsigned int h);

	Ogre::Root*			getRoot () const			{ return ogreRoot; }
	Ogre::SceneNode*	getRootNode () const		{ return sceneManager->getRootSceneNode (); }
	Ogre::SceneManager* getSceneManager () const	{ return sceneManager; }
	Ogre::RenderWindow* getRenderWindow () const	{ return renderWindow; }

	Ogre::OverlayElement*	getOverlayElement (const char* elementName) const;
	Ogre::Overlay*			getOverlay (const char* overlayName) const;
	Ogre::OverlayContainer* getContainer (const char* containerName) const;
	Ogre::OverlayManager*	getOverlayMgr () const { return overlayManager; }

	Ogre::Overlay* createOverlay (const char* name) { return overlayManager->create (name); }

	template<typename T>
	T* createOverlayElement (const char* typeName, const char* name)
	{
		return static_cast<T*>(overlayManager->createOverlayElement (typeName, name));
	}

private:
	Ogre::Root* ogreRoot;
	Ogre::SceneManager* sceneManager;
	Ogre::RenderWindow* renderWindow;
	Ogre::OverlaySystem* overlaySystem;
	Ogre::OverlayManager* overlayManager;
	
	std::string windowName;
	size_t windowWidth;
	size_t windowHeight;
};

}	// namespace Engine

#endif	// #ifndef RENDER_SYSTEM_H
