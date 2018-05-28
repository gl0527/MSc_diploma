#include "Game.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "XML/XMLParser.h"

#include "DynamicMovementProcessor.h"
#include "FPSComponent.h"
#include "WeaponComponent.h"
#include "SoldierAnimationComponent.h"
#include "AudioManager.h"
#include "PlayerDataComponent.h"
#include "ManagerComponent.h"
#include "GUIComponent.h"
#include "HUDComponent.h"


using namespace Engine;


int main (int argc, char** argv)
{
	auto& game = Game::GetInstance ();
	auto& xmlParser = XML::XMLParser::GetInstance ();
	auto& objectMgr = ObjectManager::GetInstance ();

	if (!game.Init ())
		return -1;

	new DynamicMovementProcessor;

	AudioManager::GetInstance ().SetResourceLocation ("media\\level01-arrival\\sound\\");

	if (!xmlParser.LoadXMLFromFile ("media\\level01-arrival\\map\\ShooterGame.xml"))
		return -1;

	if (auto soldierGO = objectMgr.GetGameObjectByName ("gijoe").lock ()) {
		soldierGO->AddTag ("player");

		std::shared_ptr<SoldierAnimationComponent> soldierAnimComp (new SoldierAnimationComponent ("soldierAnimComp"));
		soldierGO->AddComponent (soldierAnimComp);

		std::shared_ptr<PlayerDataComponent> soldierData (new PlayerDataComponent ("soldierData", 100));
		soldierGO->AddComponent (soldierData);
	}

	if (auto weapon = objectMgr.GetGameObjectByName ("weapon").lock ()) {
		std::shared_ptr<WeaponComponent> weaponComp (new WeaponComponent ("soldierWeapon", 1, 30));
		weapon->AddComponent (weaponComp);
	}

	if (auto gui = objectMgr.GetGameObjectByName ("gui").lock ()) {
		std::shared_ptr<GUIComponent> guiComp (new GUIComponent ("GUIComponent"));
		gui->AddComponent (guiComp);
	}

	if (auto manager = objectMgr.GetGameObjectByName ("manager").lock ()) {
		std::shared_ptr<ManagerComponent> managerComp (new ManagerComponent ("managerComp", 30));
		manager->AddComponent (managerComp);

		std::shared_ptr<HUDComponent> hudComponent (new HUDComponent ("managerHUD"));
		manager->AddComponent (hudComponent);
	}

	if (auto frames = objectMgr.CreateGameObject ("fps").lock ()) {
		std::shared_ptr<FPSComponent> fpsc (new FPSComponent ("FPS"));
		frames->AddComponent (fpsc);
	}

	if (auto gijoecam = objectMgr.GetGameObjectByName ("gijoecamera").lock ()) {
		AudioManager::GetInstance ().SetListener ("gijoecamera");
	}

	if (auto box = objectMgr.GetGameObjectByName ("doboz1").lock ())
		box->AddTag ("ammo");

	if (auto box = objectMgr.GetGameObjectByName ("doboz2").lock ())
		box->AddTag ("ammo");

	game.Start ();
	game.DeleteInstance ();

	return 0;
}