#include "FPSComponent.h"
#include "Game.h"
#include "Overlay\OgreOverlaySystem.h"
#include "OgreStringConverter.h"
#include "OgreRenderWindow.h"


FPSComponent::FPSComponent (const std::string& name)
	: Component (name),
	renderWnd (nullptr),
	text (nullptr)
{
	renderWnd = RenderSystem::GetInstance ().GetRenderWindow ();

	text = RenderSystem::GetInstance ().CreateOverlayElement<Ogre::TextAreaOverlayElement> ("TextArea", name.c_str ());
	text->setFontName ("SdkTrays/Value");
	text->setMetricsMode (Ogre::GMM_PIXELS);
	text->setCharHeight (24);

	Ogre::OverlayContainer* panel = RenderSystem::GetInstance ().CreateOverlayElement<Ogre::OverlayContainer> ("Panel", "PanelName");
	panel->setMetricsMode (Ogre::GMM_PIXELS);
	panel->setPosition (10, 10);
	panel->setDimensions (180, 50);

	Ogre::Overlay* overlay = RenderSystem::GetInstance ().CreateOverlay ("OverlayName");
	overlay->add2D (panel);

	panel->addChild (text);

	overlay->show ();
}


void FPSComponent::PostUpdate (float t, float dt)
{
	float currFPS = RenderSystem::GetInstance ().GetCurrentFPS ();
	float avgFPS = RenderSystem::GetInstance ().GetAverageFPS ();
	size_t triCount = renderWnd->getTriangleCount ();

	text->setCaption (Ogre::String ("Current FPS: ") + Ogre::StringConverter::toString (currFPS) + "\n"
		+ Ogre::String ("Average FPS: ") + Ogre::StringConverter::toString (avgFPS) + "\n"
		+ Ogre::String ("Triangles: ") + Ogre::StringConverter::toString (triCount));
}
