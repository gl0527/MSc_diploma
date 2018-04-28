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
#include "SoldierAnimationComponent.h"
#include "SoldierStateComponent.h"
#include "AudioManager.h"
#include "AudioSourceComponent.h"
#include "ParticleComponent.h"

#include "OgreSceneManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Button.h"


using namespace Engine;


static void GLOBAL_FUNC_NAME (MyGUI::Widget* _sender)
{
	if (AudioManager::GetInstance ().IsEnabled ())
		AudioManager::GetInstance ().Disable ();
	else
		AudioManager::GetInstance ().Enable ();
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

	AudioManager::GetInstance ().SetResourceLocation ("media\\level01-arrival\\sound\\");

	renderSys->CreatePlaneMeshXZ ("ground", 0, 100, 100);

	if (!xmlParser.LoadXMLFromFile ("media\\level01-arrival\\map\\ShooterGame.xml"))
		return -1;

	renderSys->LoadGUILayout ("EditPanel.layout");
	renderSys->LoadGUILayout ("MainPanel.layout");
	auto button = renderSys->GetWidget<MyGUI::Button> ("New");
	button->eventMouseButtonClick = MyGUI::newDelegate (GLOBAL_FUNC_NAME);

	if (auto exp = objectMgr.GetGameObjectByName ("explosive").lock ()) {
		if (auto explosivePhysx = exp->GetFirstComponentByType<PhysicsComponent> ().lock ()) {
			explosivePhysx->onTriggerEnter += [&] (PhysicsComponent* otherPhyComp) {
				static bool particleEnabled = true;
				
				if (otherPhyComp != nullptr) {
					if (auto explosiveParticle = exp->GetFirstComponentByType<ParticleComponent> ().lock ()) {
						explosiveParticle->SetEnabled (particleEnabled);
					}
					otherPhyComp->AddForce (2'000, 4'000, -8'000);
				}
				particleEnabled = !particleEnabled;
			};
		}
	}

	if (auto soldierGO = objectMgr.GetGameObjectByName ("human01").lock ()) {
		std::shared_ptr<SoldierStateComponent> soldierStateComp (new SoldierStateComponent ("soldierStateComp"));
		soldierGO->AddComponent (soldierStateComp);
	}

	if (auto soldierGO = objectMgr.GetGameObjectByName ("gijoe").lock ()) {
		std::shared_ptr<SoldierAnimationComponent> soldierAnimComp (new SoldierAnimationComponent ("soldierAnimComp"));
		soldierGO->AddComponent (soldierAnimComp);
	}

	if (auto weapon = objectMgr.GetGameObjectByName ("weaponNode").lock ()) {
		std::shared_ptr<WeaponComponent> weaponComp (new WeaponComponent ("soldierWeapon"));
		weapon->AddComponent (weaponComp);
	}

	if (auto frames = objectMgr.CreateGameObject ("fps").lock ()) {
		std::shared_ptr<FPSComponent> fpsc (new FPSComponent ("FPS"));
		frames->AddComponent (fpsc);
	}

	if (auto gijoecam = objectMgr.GetGameObjectByName ("gijoecamera").lock ()) {
		AudioManager::GetInstance ().SetListener ("gijoecamera");
	}

	game.Start ();
	game.DeleteInstance ();

	return 0;
}