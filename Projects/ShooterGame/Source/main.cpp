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


int main(int argc, char** argv)
{
	auto& game = Game::GetInstance();
	
	if (!game.Init())
		return -1;

	new InputProcessor;
	new DynamicMovementProcessor;

	auto renderSys = game.GetRenderSystem();
	auto& xmlParser = XML::XMLParser::GetInstance();
	auto& objectMgr = ObjectManager::GetInstance ();

	if (!xmlParser.LoadXMLFromFile ("media\\map\\test.xml"))
		return -1;

	if (auto exp = objectMgr.GetGameObjectByName("explosive").lock())
	{
		if(auto explosivePhysx = exp->getFirstComponentByType<PhysicsComponent>().lock())
			explosivePhysx->setOnTriggerEnter ([&objectMgr, &exp] (PhysicsComponent* other) { 
				if(const auto& explosiveAudio = exp->getFirstComponentByType<AudioComponent>().lock())
					explosiveAudio->play();
				objectMgr.RemoveGameObject(other->GetOwner()->GetName());
			});
	}

	if (auto soldierGO = objectMgr.GetGameObjectByName ("gijoe").lock ()) {
		std::shared_ptr<SoldierAnimComponent> soldierAnimComp (new SoldierAnimComponent ("soldierAnimComp"));

		soldierGO->AddComponent (soldierAnimComp);
	}

	if (auto lvl = objectMgr.GetGameObjectByName("level").lock())
	{
		if(auto levelSound = lvl->getFirstComponentByType<AudioComponent>().lock())
			levelSound->play();
	}

	if (auto frames = objectMgr.CreateGameObject("fps").lock())
	{
		std::shared_ptr<FPSComponent> fpsc(new FPSComponent("FPS"));
		frames->AddComponent(fpsc);
	}

	if (auto gijoecam = objectMgr.GetGameObjectByName("gijoecamera").lock())
	{
		std::shared_ptr<CameraControlComponent> camControl(new CameraControlComponent("camc"));
		gijoecam->AddComponent(camControl);
	}
	if (auto weapon = objectMgr.GetGameObjectByName("weapon").lock())
	{
		//Factory<WeaponComponent> weaponFactory;

		//weaponFactory.SetCtorParams ("weap");
		//weaponFactory.SetAllNonCtorParams ();

		//Factory<WeaponComponent>::SPtr weaponComponent = weaponFactory.Create ();
		
		//weapon->AddComponent (weaponComponent);
	}
	
	// setting up environment
	auto sceneMgr = renderSys->getSceneManager();

	sceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f, 1.0f)); // ez is kellene az xml-be
	//sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	//sceneMgr->setShadowColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	sceneMgr->setSkyBox(true, "Stormy");

	game.Start();
	game.DeleteInstance();

	return 0;
}