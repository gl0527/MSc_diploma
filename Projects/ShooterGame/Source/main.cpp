// include from Engine
#include "AudioComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "PhysicsComponent.h"
#include "XML/XMLParser.h"

#include "DynamicMovementProcessor.h"
#include "InputComponent.h"
#include "FPSComponent.h"
#include "InputProcessor.h"
#include "CameraControlComponent.h"
#include "WeaponComponent.h"
#include "SoldierAnimComponent.h"


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

	renderSys->LoadGUILayout ("EditPanel.layout");
	renderSys->LoadGUILayout ("MainPanel.layout");
	auto button = renderSys->GetWidget<MyGUI::Button> ("New");
	button->eventMouseButtonClick = MyGUI::newDelegate (GLOBAL_FUNC_NAME);

	if (auto exp = objectMgr.GetGameObjectByName ("explosive").lock ()) {
		if (auto explosivePhysx = exp->GetFirstComponentByType<PhysicsComponent> ().lock ()) {
			explosivePhysx->onTriggerEnter += [] (PhysicsComponent* otherPhyComp) {
				if (otherPhyComp != nullptr)
					otherPhyComp->AddForce (2'000, 1'000, -20'000);
			};
		}
	}

	if (auto soldierGO = objectMgr.GetGameObjectByName ("gijoe").lock ()) {
		std::shared_ptr<SoldierAnimComponent> soldierAnimComp (new SoldierAnimComponent ("soldierAnimComp"));

		soldierGO->AddComponent (soldierAnimComp);
	}

	if (auto lvl = objectMgr.GetGameObjectByName ("level").lock ()) {
		if (auto levelSound = lvl->GetFirstComponentByType<AudioComponent> ().lock ())
			levelSound->Play ();
	}

	if (auto frames = objectMgr.CreateGameObject ("fps").lock ()) {
		std::shared_ptr<FPSComponent> fpsc (new FPSComponent ("FPS"));
		frames->AddComponent (fpsc);
	}

	if (auto gijoecam = objectMgr.GetGameObjectByName ("gijoecamera").lock ()) {
		std::shared_ptr<CameraControlComponent> camControl (new CameraControlComponent ("camc"));
		gijoecam->AddComponent (camControl);
	}
	if (auto weapon = objectMgr.GetGameObjectByName ("weapon").lock ()) {
		//Factory<WeaponComponent> weaponFactory;

		//weaponFactory.SetCtorParams ("weap");
		//weaponFactory.SetAllNonCtorParams ();

		//Factory<WeaponComponent>::SPtr weaponComponent = weaponFactory.Create ();

		//weapon->AddComponent (weaponComponent);
	}

	// setting up environment
	auto sceneMgr = renderSys->GetSceneManager ();

	sceneMgr->setAmbientLight (Ogre::ColourValue (0.2f, 0.2f, 0.2f, 1.0f)); // ez is kellene az xml-be
	//sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	//sceneMgr->setShadowColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	sceneMgr->setSkyBox (true, "Stormy");

	game.Start ();
	game.DeleteInstance ();

	return 0;
}