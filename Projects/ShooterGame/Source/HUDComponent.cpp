#include "HUDComponent.h"
#include "Overlay/OgreOverlay.h"
#include "Overlay/OgreOverlayElement.h"
#include "RenderSystem.h"


HUDComponent::HUDComponent (const std::string& name) :
	Component (name),
	m_pHealthText (nullptr),
	m_pKillsText (nullptr),
	m_pAmmoText (nullptr),
	m_pRoundsText (nullptr),
	m_pTimerText (nullptr)
{
}


void HUDComponent::Start ()
{
	Ogre::Overlay* mainOverlay = RenderSystem::GetInstance ().GetOverlay ("MainOverlay");
	mainOverlay->show ();

	m_pHealthText = RenderSystem::GetInstance ().GetOverlayElement ("Health");
	m_pAmmoText = RenderSystem::GetInstance ().GetOverlayElement ("Bullet");
	m_pKillsText = RenderSystem::GetInstance ().GetOverlayElement ("Kills");
	m_pRoundsText = RenderSystem::GetInstance ().GetOverlayElement ("CurrentRounds");
	m_pTimerText = RenderSystem::GetInstance ().GetOverlayElement ("Timer");
}


void HUDComponent::SetHealthText (const std::string& healthText)
{
	if (m_pHealthText != nullptr)
		m_pHealthText->setCaption (healthText);
}


void HUDComponent::SetKillsText (const std::string& killsText)
{
	if (m_pKillsText != nullptr)
		m_pKillsText->setCaption (killsText);
}


void HUDComponent::SetAmmoText (const std::string& ammoText)
{
	if (m_pAmmoText != nullptr)
		m_pAmmoText->setCaption (ammoText);
}


void HUDComponent::SetRoundsText (const std::string& roundsText)
{
	if (m_pRoundsText != nullptr)
		m_pRoundsText->setCaption (roundsText);
}


void HUDComponent::SetTimerText (const std::string& timerText)
{
	if (m_pTimerText != nullptr)
		m_pTimerText->setCaption (timerText);
}
