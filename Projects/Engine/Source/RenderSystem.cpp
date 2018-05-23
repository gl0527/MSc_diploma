#include "RenderSystem.h"
#include "Overlay\OgreOverlaySystem.h"
#include "MyGUI_OgrePlatform.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_LayoutManager.h"
#include "InputManager.h"
#include "ErrorMessage.h"


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
	m_wndHeight (h),
	m_currentFPS (0.0f),
	m_averageFPS (0.0f)
{
}


RenderSystem& RenderSystem::GetInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (new RenderSystem ("Project lab Gurzo Lajos")); });
	return *s_pInstance.get ();
}


void RenderSystem::Init ()
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

	Ogre::String resourcePath, resourceType;

	for (auto it = configFile.getSectionIterator (); it.hasMoreElements (); ) {
		Ogre::String resourceGroupName = it.peekNextKey ();
		auto* settings = it.getNext ();

		for (auto setIt = settings->begin (); setIt != settings->end (); ++setIt) {
			resourceType = setIt->first;
			resourcePath = setIt->second;

			resGroupManager->addResourceLocation (resourcePath, resourceType, resourceGroupName);
		}
	}

	if (!m_pOgreRoot->restoreConfig () && !m_pOgreRoot->showConfigDialog ()) {
		delete m_pOgreRoot;
		return;
	}

	m_pOgreRoot->initialise (false);

	m_pRenderWnd = m_pOgreRoot->createRenderWindow (m_wndName, m_wndWidth, m_wndHeight, false);
	m_pSceneMgr = m_pOgreRoot->createSceneManager (Ogre::ST_GENERIC);

	m_pOverlaySys = new Ogre::OverlaySystem;
	m_pSceneMgr->addRenderQueueListener (m_pOverlaySys);
	m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr ();

	resGroupManager->initialiseAllResourceGroups ();
	
	m_pOgrePlatform = new MyGUI::OgrePlatform;
	m_pOgrePlatform->initialise (m_pRenderWnd, m_pSceneMgr, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

	m_pGUI = new MyGUI::Gui;
	m_pGUI->initialise ();
}


void RenderSystem::Start ()
{
	InputManager::GetInstance ().AddGUIAsKeyListener (this);
	InputManager::GetInstance ().AddGUIAsMouseListener (this);
}


void RenderSystem::Update (float t, float dt)
{
	static float sumFPS = 0.0f;
	static unsigned int count = 0;
	static float lastT = 0.0f;
	
	if (m_pRenderWnd->isClosed ()) {
		m_pOgreRoot->shutdown ();
		return;
	}

	Ogre::WindowEventUtilities::messagePump ();

	if (t - lastT > 0.99f) {
		lastT = t;
		m_currentFPS = 1 / dt;
		sumFPS += m_currentFPS;
		++count;
		m_averageFPS = sumFPS / count;
	}

	if (!m_pOgreRoot->renderOneFrame ()) {
		m_pOgreRoot->shutdown ();
		return;
	}
}


void RenderSystem::Destroy ()
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


void RenderSystem::LoadResourceGroup (const std::string& resGroupName)
{
	try {
		Ogre::ResourceGroupManager::getSingletonPtr ()->loadResourceGroup (resGroupName);
	} catch (const Ogre::ItemIdentityException& iie) {
		ERR_LOG (std::cerr, iie.what ());
	}
}


void RenderSystem::UnloadResourceGroup (const std::string& resGroupName)
{
	try {
		Ogre::ResourceGroupManager::getSingletonPtr ()->unloadResourceGroup (resGroupName);
	} catch (const Ogre::ItemIdentityException& iie) {
		ERR_LOG (std::cerr, iie.what());
	}
}


void RenderSystem::SetActiveViewport ()
{
	m_pOgrePlatform->getRenderManagerPtr ()->setActiveViewport (0);
}


bool RenderSystem::mouseMoved (const OIS::MouseEvent& me)
{
	return MyGUI::InputManager::getInstance().injectMouseMove (me.state.X.abs, me.state.Y.abs, me.state.Z.abs);
}


bool RenderSystem::mousePressed (const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	return MyGUI::InputManager::getInstance().injectMousePress (me.state.X.abs, me.state.Y.abs, MyGUI::MouseButton::Enum(id));
}


bool RenderSystem::mouseReleased (const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	return MyGUI::InputManager::getInstance().injectMouseRelease (me.state.X.abs, me.state.Y.abs, MyGUI::MouseButton::Enum(id));
}


bool RenderSystem::keyPressed (const OIS::KeyEvent& ke)
{
	return MyGUI::InputManager::getInstance().injectKeyPress (MyGUI::KeyCode::Enum(ke.key), ke.text);
}


bool RenderSystem::keyReleased (const OIS::KeyEvent& ke)
{
	return MyGUI::InputManager::getInstance().injectKeyRelease (MyGUI::KeyCode::Enum(ke.key));
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


float RenderSystem::GetCurrentFPS () const
{
	return m_currentFPS;
}


float RenderSystem::GetAverageFPS () const
{
	return m_averageFPS;
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


MyGUI::VectorWidgetPtr RenderSystem::LoadGUILayout (const std::string& layoutFileName)
{
	return MyGUI::LayoutManager::getInstance().loadLayout (layoutFileName);
}

}	// namespace Engine
