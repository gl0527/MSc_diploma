#include "RenderSystem.h"


namespace Engine {

RenderSystem::RenderSystem (const char* wName, size_t w, size_t h)
	: ogreRoot (nullptr),
	sceneManager (nullptr),
	renderWindow (nullptr),
	overlaySystem (nullptr),
	overlayManager (nullptr),
	windowName (wName),
	windowWidth (w),
	windowHeight (h)
{
}


bool RenderSystem::init ()
{
#ifdef _DEBUG
	m_resConfig = "resources_d.cfg";
	m_plugConfig = "plugins_d.cfg";
#else
	m_resConfig = "resources.cfg";
	m_plugConfig = "plugins.cfg";
#endif

	ogreRoot = new Ogre::Root (m_plugConfig);

	Ogre::ConfigFile configFile;
	configFile.load (m_resConfig);

	Ogre::ResourceGroupManager* resGroupManager = Ogre::ResourceGroupManager::getSingletonPtr ();

	Ogre::String name, locType;
	for (auto it = configFile.getSectionIterator (); it.hasMoreElements (); ) {
		auto* settings = it.getNext ();

		for (auto setIt = settings->begin (); setIt != settings->end (); ++setIt) {
			locType = setIt->first;
			name = setIt->second;

			resGroupManager->addResourceLocation (name, locType);
		}
	}

	if (!ogreRoot->restoreConfig () && !ogreRoot->showConfigDialog ()) {
		delete ogreRoot;
		return false;
	}

	ogreRoot->initialise (false);

	renderWindow = ogreRoot->createRenderWindow (windowName, windowWidth, windowHeight, false);
	sceneManager = ogreRoot->createSceneManager (Ogre::ST_GENERIC);

	overlaySystem = new Ogre::OverlaySystem;
	sceneManager->addRenderQueueListener (overlaySystem);
	overlayManager = Ogre::OverlayManager::getSingletonPtr ();

	resGroupManager->initialiseAllResourceGroups ();

	return true;
}


bool RenderSystem::update (float t, float dt)
{
	if (renderWindow->isClosed ()) {
		ogreRoot->shutdown ();
		return false;
	}

	Ogre::WindowEventUtilities::messagePump ();

	if (!ogreRoot->renderOneFrame ()) {
		ogreRoot->shutdown ();
		return false;
	}
	return true;
}


void RenderSystem::destroy ()
{
	if (overlaySystem) {
		delete overlaySystem;
		overlaySystem = nullptr;
	}
	if (sceneManager) {
		sceneManager->clearScene ();
	}
	if (renderWindow) {
		renderWindow->destroy ();
	}
	if (ogreRoot) {
		delete ogreRoot;
		ogreRoot = nullptr;
	}
}


Ogre::MeshPtr RenderSystem::createPlaneMeshXZ (const char* planeMeshName, float y, unsigned int u, unsigned int v)
{
	Ogre::MovablePlane plane (planeMeshName);
	plane.d = y;
	plane.normal = Ogre::Vector3::UNIT_Y;

	Ogre::MeshPtr mp = Ogre::MeshManager::getSingleton ().createPlane (
		planeMeshName,
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		1,// x-size
		1,// z-size
		1,// x-segments
		1,// z-segments
		true,
		1,
		u, // u-tile
		v, // v-tile
		Ogre::Vector3::UNIT_Z);

	return mp;
}


Ogre::TexturePtr RenderSystem::createTexture (const char* texName, unsigned int w, unsigned int h)
{
	Ogre::TexturePtr tp = Ogre::TextureManager::getSingleton ().createManual (
		texName,
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D,
		w,
		h,
		0,
		Ogre::PF_R8G8B8,
		Ogre::TU_RENDERTARGET);

	return tp;
}


Ogre::OverlayElement* RenderSystem::getOverlayElement (const char* elementName) const
{
	return overlayManager->getOverlayElement (elementName);
}


Ogre::Overlay* RenderSystem::getOverlay (const char* overlayName) const
{
	return overlayManager->getByName (overlayName);
}


Ogre::OverlayContainer* RenderSystem::getContainer (const char* containerName) const
{
	return static_cast<Ogre::OverlayContainer*>(overlayManager->getOverlayElement (containerName));
}

}	// namespace Engine
