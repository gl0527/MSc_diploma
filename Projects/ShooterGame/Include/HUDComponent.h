#pragma once

#ifndef HUD_COMPONENT_H
#define HUD_COMPONENT_H

#include "Component.h"

using namespace Engine;

namespace Ogre {

class OverlayElement;

}	// namespace Ogre


class HUDComponent : public Component
{
public:
	explicit HUDComponent (const std::string& name);

	void Start () override;

	void SetHealthText (const std::string& healthText);
	void SetKillsText (const std::string& killsText);
	void SetAmmoText (const std::string& ammoText);
	void SetRoundsText (const std::string& roundsText);
	void SetTimerText (const std::string& timerText);

private:
	Ogre::OverlayElement* m_pHealthText;
	Ogre::OverlayElement* m_pKillsText;
	Ogre::OverlayElement* m_pAmmoText;
	Ogre::OverlayElement* m_pRoundsText;
	Ogre::OverlayElement* m_pTimerText;
};


#endif	// #ifndef HUD_COMPONENT_H
