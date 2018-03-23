// include from Engine
#include "Game.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "PhysicsComponent.h"
#include "XML/XMLParser.h"

#include "DynamicMovementProcessor.h"
#include "InputComponent.h"
#include "FPSComponent.h"
#include "InputProcessor.h"
#include "WeaponComponent.h"
#include "SoldierAnimComponent.h"
#include "AudioManager.h"
#include "AudioSourceComponent.h"

#include "OgreSceneManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Button.h"


using namespace Engine;


static void GLOBAL_FUNC_NAME (MyGUI::Widget* _sender)
{
	std::cout << "Button pressed.\n";
	//Game::GetInstance().GetRenderSystem ()->UnloadResourceGroup ("General");
}


int main(int argc, char** argv)
{
#ifndef __cplusplus
#error C++ compiler required
#endif

	auto& game = Game::GetInstance();
	auto renderSys = game.GetRenderSystem ();
	auto& xmlParser = XML::XMLParser::GetInstance ();
	auto& objectMgr = ObjectManager::GetInstance ();
	
	if (!game.Init ())
		return -1;

	new InputProcessor;
	new DynamicMovementProcessor;

	if (!xmlParser.LoadXMLFromFile ("media\\level01-arrival\\map\\test.xml"))
		return -1;

	AudioManager::GetInstance ().SetPathToBuffers ("media\\level01-arrival\\sound\\");

	renderSys->LoadGUILayout ("EditPanel.layout");
	renderSys->LoadGUILayout ("MainPanel.layout");
	auto button = renderSys->GetWidget<MyGUI::Button> ("New");
	button->eventMouseButtonClick = MyGUI::newDelegate (GLOBAL_FUNC_NAME);

	if (auto exp = objectMgr.GetGameObjectByName ("explosive").lock ()) {
		if (auto explosivePhysx = exp->GetFirstComponentByType<PhysicsComponent> ().lock ()) {
			explosivePhysx->onTriggerEnter += [] (PhysicsComponent* otherPhyComp) {
				if (otherPhyComp != nullptr) {
					otherPhyComp->AddForce (2'000, 1'000, -20'000);
				}
			};
		}
	}

	if (auto soldierGO = objectMgr.GetGameObjectByName ("gijoe").lock ()) {
		std::shared_ptr<SoldierAnimComponent> soldierAnimComp (new SoldierAnimComponent ("soldierAnimComp"));

		soldierGO->AddComponent (soldierAnimComp);
	}

	if (auto lvl = objectMgr.GetGameObjectByName ("level").lock ()) {
		std::shared_ptr<AudioSourceComponent> audio (new AudioSourceComponent ("music", AudioSourceComponent::Music));
		audio->AddBuffer ("main_theme_2.wav");
		audio->AddBuffer ("main_theme_4.wav");

		lvl->AddComponent (audio);

		audio->SetLooping (true);
		audio->SetVolume (0.3f);
		audio->Play ();
	}

	if (auto ball = objectMgr.GetGameObjectByName ("ball").lock ()) {
		std::shared_ptr<AudioSourceComponent> audio (new AudioSourceComponent ("levelKeszo", AudioSourceComponent::SoundEffect));
		audio->AddBuffer ("churchbell.wav");
		audio->AddBuffer ("bounce.wav");

		ball->AddComponent (audio);

		audio->SetLooping (true);
		audio->SetMaxDistanceWithFullGain (40.0f);
		audio->SetMinDistanceWithZeroGain (150.0f);
		audio->Play ();
	}

	if (auto frames = objectMgr.CreateGameObject ("fps").lock ()) {
		std::shared_ptr<FPSComponent> fpsc (new FPSComponent ("FPS"));
		frames->AddComponent (fpsc);
	}

	if (auto gijoecam = objectMgr.GetGameObjectByName ("gijoecamera").lock ()) {
		AudioManager::GetInstance ().SetListener ("gijoecamera");
	}

	// setting up environment
	auto sceneMgr = renderSys->GetSceneManager ();

	sceneMgr->setAmbientLight (Ogre::ColourValue (0.4f, 0.4f, 0.4f, 1.0f)); // ez is kellene az xml-be
	//sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	//sceneMgr->setShadowColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	sceneMgr->setSkyBox (true, "Stormy");

	game.Start ();
	game.DeleteInstance ();

	return 0;
}