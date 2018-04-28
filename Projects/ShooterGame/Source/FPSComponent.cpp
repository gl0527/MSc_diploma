#include "FPSComponent.h"
#include "Game.h"
#include "Overlay\OgreOverlaySystem.h"
#include "OgreStringConverter.h"
#include "OgreRenderWindow.h"


FPSComponent::FPSComponent (const std::string& name)
	: Component (name),
	renderSys (nullptr),
	renderWnd (nullptr),
	text (nullptr)
{
	renderSys = Game::GetInstance ().GetRenderSystem ();
	renderWnd = renderSys->GetRenderWindow ();

	text = renderSys->CreateOverlayElement<Ogre::TextAreaOverlayElement> ("TextArea", name.c_str ());
	text->setFontName ("SdkTrays/Value");
	text->setMetricsMode (Ogre::GMM_PIXELS);
	text->setCharHeight (24);

	Ogre::OverlayContainer* panel = renderSys->CreateOverlayElement<Ogre::OverlayContainer> ("Panel", "PanelName");
	panel->setMetricsMode (Ogre::GMM_PIXELS);
	panel->setPosition (10, 10);
	panel->setDimensions (180, 50);

	Ogre::Overlay* overlay = renderSys->CreateOverlay ("OverlayName");
	overlay->add2D (panel);

	panel->addChild (text);

	overlay->show ();
}


FPSComponent::~FPSComponent ()
{
}


void FPSComponent::PostUpdate (float t, float dt)
{
	float fps = renderWnd->getLastFPS ();
	size_t triCount = renderWnd->getTriangleCount ();
	text->setCaption (Ogre::String ("Frames/sec: ") + Ogre::StringConverter::toString (fps) + "\n"
		+ Ogre::String ("Triangles: ") + Ogre::StringConverter::toString (triCount));
}
