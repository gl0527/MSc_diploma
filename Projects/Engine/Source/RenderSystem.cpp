#include "RenderSystem.h"
#include "Overlay\OgreOverlaySystem.h"
#include "MyGUI_OgrePlatform.h"
#include "InputManager.h"


namespace Engine {

RenderSystem::RenderSystem (const char* wName, size_t w, size_t h)
	: m_pOgreRoot (nullptr),
	m_pSceneMgr (nullptr),
	m_pRenderWnd (nullptr),
	m_pOverlaySys (nullptr),
	m_pOverlayMgr (nullptr),
	m_pOgrePlatform (nullptr),
	m_pGUI (nullptr),
	m_wndName (wName),
	m_wndWidth (w),
	m_wndHeight (h)
{
}


bool RenderSystem::init ()
{
#ifdef _DEBUG
	std::string resourceCfg ("resources_d.cfg");
	std::string pluginCfg ("plugins_d.cfg");
#else
	std::string resourceCfg ("resources.cfg");
	std::string pluginCfg ("plugins.cfg");
#endif

	m_pOgreRoot = new Ogre::Root (pluginCfg);

	Ogre::ConfigFile configFile;
	configFile.load (resourceCfg);

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

	if (!m_pOgreRoot->restoreConfig () && !m_pOgreRoot->showConfigDialog ()) {
		delete m_pOgreRoot;
		return false;
	}

	m_pOgreRoot->initialise (false);

	m_pRenderWnd = m_pOgreRoot->createRenderWindow (m_wndName, m_wndWidth, m_wndHeight, false);
	m_pSceneMgr = m_pOgreRoot->createSceneManager (Ogre::ST_GENERIC);

	m_pOverlaySys = new Ogre::OverlaySystem;
	m_pSceneMgr->addRenderQueueListener (m_pOverlaySys);
	m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr ();

	resGroupManager->initialiseAllResourceGroups ();

	m_pOgrePlatform = new MyGUI::OgrePlatform;
	m_pOgrePlatform->initialise (m_pRenderWnd, m_pSceneMgr);
	m_pGUI = new MyGUI::Gui;
	m_pGUI->initialise ();

	return true;
}


bool RenderSystem::Start ()
{
	InputManager::GetInstance ().AddKeyListener (this, "RenderSystemKL");
	InputManager::GetInstance ().AddMouseListener (this, "RenderSystemML");

	return true;
}


void RenderSystem::SetOgrePlatform ()
{
	m_pOgrePlatform->getRenderManagerPtr ()->setActiveViewport (0);
}


bool RenderSystem::update (float t, float dt)
{
	if (m_pRenderWnd->isClosed ()) {
		m_pOgreRoot->shutdown ();
		return false;
	}

	Ogre::WindowEventUtilities::messagePump ();

	if (!m_pOgreRoot->renderOneFrame ()) {
		m_pOgreRoot->shutdown ();
		return false;
	}
	return true;
}


void RenderSystem::destroy ()
{
	if (m_pGUI != nullptr) {
		m_pGUI->shutdown ();
		delete m_pGUI;
		m_pGUI = nullptr;
	}
	if (m_pOgrePlatform != nullptr) {
		m_pOgrePlatform->shutdown ();
		delete m_pOgrePlatform;
		m_pOgrePlatform = nullptr;
	}
	if (m_pOverlaySys != nullptr) {
		delete m_pOverlaySys;
		m_pOverlaySys = nullptr;
	}
	if (m_pSceneMgr != nullptr) {
		m_pSceneMgr->clearScene ();
	}
	if (m_pRenderWnd != nullptr) {
		m_pRenderWnd->destroy ();
	}
	if (m_pOgreRoot != nullptr) {
		delete m_pOgreRoot;
		m_pOgreRoot = nullptr;
	}
}


bool RenderSystem::mouseMoved (const OIS::MouseEvent& me)
{
	MyGUI::InputManager::getInstance().injectMouseMove (me.state.X.abs, me.state.Y.abs, me.state.Z.abs);

	return true;
}


bool RenderSystem::mousePressed (const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	MyGUI::InputManager::getInstance().injectMousePress (me.state.X.abs, me.state.Y.abs, MyGUI::MouseButton::Enum(id));

	return true;
}


bool RenderSystem::mouseReleased (const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	MyGUI::InputManager::getInstance().injectMouseRelease (me.state.X.abs, me.state.Y.abs, MyGUI::MouseButton::Enum(id));

	return true;
}


bool RenderSystem::keyPressed (const OIS::KeyEvent& ke)
{
	MyGUI::InputManager::getInstance().injectKeyPress (MyGUI::KeyCode::Enum(ke.key), ke.text);

	return true;
}


bool RenderSystem::keyReleased (const OIS::KeyEvent& ke)
{
	MyGUI::InputManager::getInstance().injectKeyRelease (MyGUI::KeyCode::Enum(ke.key));

	return true;
}


Ogre::MeshPtr RenderSystem::CreatePlaneMeshXZ (const char* planeMeshName, float y, unsigned int u, unsigned int v)
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


Ogre::TexturePtr RenderSystem::CreateTexture (const char* texName, unsigned int w, unsigned int h)
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


Ogre::Root* RenderSystem::GetRoot () const
{
	return m_pOgreRoot;
}


Ogre::SceneNode* RenderSystem::GetRootNode () const
{
	return m_pSceneMgr->getRootSceneNode ();
}


Ogre::SceneManager* RenderSystem::GetSceneManager () const
{
	return m_pSceneMgr;
}


Ogre::RenderWindow* RenderSystem::GetRenderWindow () const
{
	return m_pRenderWnd;
}


Ogre::OverlayElement* RenderSystem::GetOverlayElement (const char* elementName) const
{
	return m_pOverlayMgr->getOverlayElement (elementName);
}


Ogre::Overlay* RenderSystem::GetOverlay (const char* overlayName) const
{
	return m_pOverlayMgr->getByName (overlayName);
}


Ogre::OverlayContainer* RenderSystem::GetContainer (const char* containerName) const
{
	return static_cast<Ogre::OverlayContainer*>(m_pOverlayMgr->getOverlayElement (containerName));
}


Ogre::OverlayManager* RenderSystem::GetOverlayMgr () const
{
	return m_pOverlayMgr;
}


Ogre::Overlay* RenderSystem::CreateOverlay (const char* name)
{
	return m_pOverlayMgr->create (name);
}


void RenderSystem::LoadGUILayout (const std::string& layoutFileName)
{
	MyGUI::LayoutManager::getInstance().loadLayout (layoutFileName);
}

}	// namespace Engine
