#include "GUIComponent.h"
#include "Game.h"
#include "RenderSystem.h"
#include "MyGUI_Button.h"
#include "MyGUI_PointerManager.h"
#include "AudioManager.h"


namespace {

void OnQuit (MyGUI::Widget* _sender, int left, int top, MyGUI::MouseButton mb)
{
	Game::GetInstance ().Destroy ();
}


void OnContinue (MyGUI::Widget* _sender)
{
	AudioManager::GetInstance ().Enable ();
	Game::GetInstance ().Continue ();
}

}	// namespace


GUIComponent::GUIComponent (const std::string& name) :
	Component (name)
{
}


void GUIComponent::Start ()
{
	m_menu = RenderSystem::GetInstance ().LoadGUILayout ("MainPanel.layout");
	m_pContinueButton = RenderSystem::GetInstance ().GetWidget<MyGUI::Button> ("New");
	m_pQuitButton = RenderSystem::GetInstance ().GetWidget<MyGUI::Button> ("Quit");

	m_pContinueButton->eventMouseButtonClick = MyGUI::newDelegate (OnContinue);
	m_pQuitButton->eventMouseButtonReleased = MyGUI::newDelegate (OnQuit);
}


void GUIComponent::PostUpdate (float t, float dt)
{
	bool isGameStopped = Game::GetInstance ().GetState () == Game::State::Stopped;

	for (auto menuItem : m_menu)
		menuItem->setVisible (isGameStopped);

	MyGUI::PointerManager::getInstance().setVisible (isGameStopped);
}
