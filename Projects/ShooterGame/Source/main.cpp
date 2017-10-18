// Engine
#include "AudioComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "PhysicsComponent.h"
#include "XMLParser.h"

#include "InputComponent.h"
#include "FPSComponent.h"
#include "InputProcessor.h"
#include "DynamicMovementComponent.h"
#include "SoldierComponent.h"
#include "DynamicMovementProcessor.h"
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
	auto& xmlParser = XMLParser::GetInstance();
	auto& objectMgr = ObjectManager::GetSingletonInstance ();

	renderSys->createPlaneMeshXZ("ground", 0, 10, 10);
	auto rtt = renderSys->createTexture("sepia", 100, 100)->getBuffer()->getRenderTarget();

	/*const auto& tree = objectMgr.createGameObject("tree");
	tree->transform()->setPosition(Ogre::Vector3(-30.0f, 0.0f, -900.0f));
	tree->transform()->setScale(Ogre::Vector3(5, 5, 5));
	std::shared_ptr<MeshComponent> ball2Renderer(new MeshComponent("treeTrunk", "treetrunk.mesh"));
	std::shared_ptr<BillboardComponent> ball2bb(new BillboardComponent("bboard"));
	ball2bb->getBillboardSet()->setBillboardType(Ogre::BBT_PERPENDICULAR_SELF);
	ball2bb->getBillboardSet()->setMaterialName("TreeLeaves");
	ball2bb->getBillboardSet()->setSortingEnabled(true);
	ball2bb->getBillboardSet()->setCastShadows(true);

	for (int i = 0; i < 10; i++)
	{
		float h = Ogre::Math::RangeRandom(30, 60);
		Ogre::Vector3 pos(0, h, 0);
		Ogre::Vector3 dir(Ogre::Math::RangeRandom(-0.6f, 0.6f), 1, Ogre::Math::RangeRandom(-0.6f, 0.6f));
		dir.normalise();
		auto bb = ball2bb->getBillboardSet()->createBillboard(pos);
		bb->setDimensions(90.0f - h, 90.0f - h);
		bb->mDirection = dir;
	}

	tree->addComponent(ball2Renderer);
	tree->addComponent(ball2bb);*/

	if (!xmlParser.LoadXMLFromFile ("Media\\map\\test.xml"))
		return -1;

	if (auto& exp = objectMgr.GetGameObjectByName("explosive").lock())
	{
		if(auto& explosivePhysx = exp->getFirstComponentByType<PhysicsComponent>().lock())
			explosivePhysx->setOnTriggerEnter([&objectMgr, &exp](PhysicsComponent* other) { 
				if(const auto& explosiveAudio = exp->getFirstComponentByType<AudioComponent>().lock())
					explosiveAudio->play();
				objectMgr.RemoveGameObject(other->GetOwner()->GetName());
			});
	}

	if (auto& sld = objectMgr.GetGameObjectByName("gijoe").lock())
	{
		std::shared_ptr<AnimationComponent> anim(new AnimationComponent("soldierAnimation", Ogre::ANIMBLEND_CUMULATIVE));
		std::shared_ptr<SoldierComponent> data(new SoldierComponent("soldierComp"));

		sld->AddComponent(anim);
		sld->AddComponent(data);
	}
	
	if (auto& soldierGO = objectMgr.GetGameObjectByName ("soldier").lock ()) {
		std::shared_ptr<SoldierAnimComponent> soldierAnimComp (new SoldierAnimComponent ("soldierAnimComp"));

		soldierGO->AddComponent (soldierAnimComp);
	}

	if (auto& lvl = objectMgr.GetGameObjectByName("level").lock())
	{
		if(auto& levelSound = lvl->getFirstComponentByType<AudioComponent>().lock())
			levelSound->play();
	}

	if (auto& frames = objectMgr.CreateGameObject("fps").lock())
	{
		std::shared_ptr<FPSComponent> fpsc(new FPSComponent("FPS"));
		frames->AddComponent(fpsc);
	}

	if (auto& gijoecam = objectMgr.GetGameObjectByName("gijoecamera").lock())
	{
		std::shared_ptr<CameraControlComponent> camControl(new CameraControlComponent("camc"));
		gijoecam->AddComponent(camControl);
	}
	if (auto& weapon = objectMgr.GetGameObjectByName("weapon").lock())
	{
		std::shared_ptr<WeaponComponent> weap(new WeaponComponent("weap"));
		weapon->AddComponent(weap);
	}
	
	// setting up environment
	auto sceneMgr = renderSys->getSceneManager();

	sceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f, 1.0f)); // ez is kellene az xml-be
	//sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	//sceneMgr->setShadowColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	sceneMgr->setSkyBox(true, "Sky");

	game.Start();
	game.DeleteInstance();

	return 0;
}