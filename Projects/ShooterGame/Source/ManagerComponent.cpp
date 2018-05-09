#include "ManagerComponent.h"
#include "Prefab/GameObjectCreator.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "PlayerDataComponent.h"
#include "EnemyAIComponent.h"
#include "EnemyAnimationComponent.h"


ManagerComponent::ManagerComponent (const std::string& name) :
	Component (name)
{
}


void ManagerComponent::Start ()
{
	if (!ObjectManager::GetInstance ().GetGameObjectCreator ("pacmanPrefab", m_pPacManPrefab)) {
		m_owner->RemoveComponent (m_name);
		return;
	}
	if (!ObjectManager::GetInstance ().GetGameObjectCreator ("ghostPrefab", m_pGhostPrefab)) {
		m_owner->RemoveComponent (m_name);
		return;
	}
	if (!ObjectManager::GetInstance ().GetGameObjectCreator ("bulletPrefab", m_pBulletPrefab)) {
		m_owner->RemoveComponent (m_name);
		return;
	}
}


void ManagerComponent::PreUpdate (float t, float dt)
{
	float fractPart, intPart;
	fractPart = modf (t, &intPart);

	static unsigned int counter = 0;

	if (fmod (intPart, 30.0) < 0.01f && fractPart > 0.98) {
		CreatePacMan (counter);
		CreateGhost (counter);
		++counter;
	}
}


void ManagerComponent::CreatePacMan (unsigned int counter)
{
	std::string objectName ("pacman" + std::to_string (counter));
	m_pPacManPrefab->Instantiate (objectName);
	auto pacman = ObjectManager::GetInstance ().GetGameObjectByName (objectName).lock ();
	pacman->AddTag ("enemy");
	
	std::shared_ptr<TransformComponent> pPacmanTransform (new TransformComponent ("pacmanTransform" + std::to_string (counter)));
	pacman->AddTransformComponent (pPacmanTransform);
	pPacmanTransform->SetGlobalPosition (Ogre::Vector3 (20.0f, 0.0f, -611.0f));
	
	std::shared_ptr<PlayerDataComponent> pacmanData (new PlayerDataComponent ("pacmanData", 3));
	pacman->AddComponent (pacmanData);
	
	std::shared_ptr<EnemyAIComponent> pacmanAI (new EnemyAIComponent ("pacmanAI"));
	pacman->AddComponent (pacmanAI);
	
	std::shared_ptr<EnemyAnimationComponent> pacmanAnim (new EnemyAnimationComponent ("pacmanAnim", "walk", "go", "death"));
	pacman->AddComponent (pacmanAnim);
	
	pacman->Start ();
}


void ManagerComponent::CreateGhost (unsigned int counter)
{
	std::string objectName ("ghost" + std::to_string (counter));
	m_pGhostPrefab->Instantiate (objectName);
	auto ghost = ObjectManager::GetInstance ().GetGameObjectByName (objectName).lock ();
	ghost->AddTag ("enemy");

	std::shared_ptr<TransformComponent> pGhostTransform (new TransformComponent ("ghostTransform" + std::to_string (counter)));
	ghost->AddTransformComponent (pGhostTransform);
	pGhostTransform->SetGlobalPosition (Ogre::Vector3 (0.0f, 0.0f, -750.0f));
	pGhostTransform->SetGlobalScale (Ogre::Vector3 (3.0f, 3.0f, 3.0f));

	std::shared_ptr<PlayerDataComponent> ghostData (new PlayerDataComponent ("ghostData", 3));
	ghost->AddComponent (ghostData);

	std::shared_ptr<EnemyAIComponent> ghostAI (new EnemyAIComponent ("ghostAI"));
	ghost->AddComponent (ghostAI);

	std::shared_ptr<EnemyAnimationComponent> ghostAnim (new EnemyAnimationComponent ("ghostAnim", "walk", "freeze", "death"));
	ghost->AddComponent (ghostAnim);

	ghost->Start ();
}

