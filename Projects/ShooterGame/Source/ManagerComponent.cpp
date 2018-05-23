#include "ManagerComponent.h"
#include "Prefab/GameObjectCreator.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "PlayerDataComponent.h"
#include "EnemyAIComponent.h"
#include "EnemyAnimationComponent.h"
#include "HUDComponent.h"
#include "WeaponComponent.h"


ManagerComponent::ManagerComponent (const std::string& name, unsigned int spawningPeriod) :
	Component (name),
	m_SpawningPeriod (spawningPeriod),
	m_kills (0),
	m_round (0)
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
	
	if (auto player = ObjectManager::GetInstance ().GetGameObjectByName ("gijoe").lock ()) {
		m_pPlayer = player;
		if (auto data = player->GetFirstComponentByType<PlayerDataComponent> ().lock ())
			m_pPlayerData = data;
	}

	if (m_pPlayer == nullptr || m_pPlayerData == nullptr)
		m_owner->RemoveComponent (m_name);

	if (auto ownerHUD = m_owner->GetFirstComponentByType<HUDComponent> ().lock ())
		m_pOwnerHUD = ownerHUD;

	if (m_pOwnerHUD == nullptr)
		m_owner->RemoveComponent (m_name);
}


void ManagerComponent::PreUpdate (float t, float dt)
{
	static unsigned int lastT = 0;
	unsigned int timeFromLastWave = (unsigned int)t - lastT;

	if (t < 0.01f || timeFromLastWave > m_SpawningPeriod) {
		lastT = (unsigned int)t;
		++m_round;
		for (unsigned int i = 0; i < m_round; ++i) {
			unsigned int uniqueID = 10000 * m_round + i;
			CreatePacMan (uniqueID);
			CreateGhost (uniqueID);
		}
	}

	if (m_pPlayerData->HasWeapon ()) {
		if (auto weapon = m_pPlayer->GetChild ("weapon")) {
			if (auto weaponComp = weapon->GetFirstComponentByType<WeaponComponent> ().lock ())
				m_pPlayerWeapon = weaponComp;
		}
	}

	if (m_pOwnerHUD != nullptr) {
		if (m_pPlayerWeapon != nullptr)
			m_pOwnerHUD->SetAmmoText (std::to_string (m_pPlayerWeapon->GetAmmo ()));

		m_pOwnerHUD->SetHealthText (std::to_string (m_pPlayerData->GetHealthPoint ()));
		m_pOwnerHUD->SetKillsText (std::to_string (m_kills));
		m_pOwnerHUD->SetRoundsText (std::to_string (m_round));
		m_pOwnerHUD->SetTimerText (std::to_string (m_SpawningPeriod - timeFromLastWave));
	}
}


void ManagerComponent::CreatePacMan (unsigned int counter)
{
	if (auto pacmanSpawner = ObjectManager::GetInstance ().GetGameObjectByName ("pacmanSpawner").lock ()) {
		std::string objectName ("pacman" + std::to_string (counter));
		m_pPacManPrefab->Instantiate (objectName);
		auto pacman = ObjectManager::GetInstance ().GetGameObjectByName (objectName).lock ();
		pacman->AddTag ("enemy");

		std::shared_ptr<TransformComponent> pPacmanTransform (new TransformComponent ("pacmanTransform" + std::to_string (counter)));
		pacman->AddTransformComponent (pPacmanTransform);
		pPacmanTransform->SetGlobalPosition (pacmanSpawner->Transform ()->GetGlobalPosition ());

		std::shared_ptr<PlayerDataComponent> pacmanData (new PlayerDataComponent ("pacmanData", 2));
		pacman->AddComponent (pacmanData);

		std::shared_ptr<EnemyAIComponent> pacmanAI (new EnemyAIComponent ("pacmanAI"));
		pacman->AddComponent (pacmanAI);

		std::shared_ptr<EnemyAnimationComponent> pacmanAnim (new EnemyAnimationComponent ("pacmanAnim", "walk", "go", "death"));
		pacman->AddComponent (pacmanAnim);

		pacman->Start ();
	}
}


void ManagerComponent::CreateGhost (unsigned int counter)
{
	if (auto ghostSpawner = ObjectManager::GetInstance ().GetGameObjectByName ("ghostSpawner").lock ()) {
		std::string objectName ("ghost" + std::to_string (counter));
		m_pGhostPrefab->Instantiate (objectName);
		auto ghost = ObjectManager::GetInstance ().GetGameObjectByName (objectName).lock ();
		ghost->AddTag ("enemy");

		std::shared_ptr<TransformComponent> pGhostTransform (new TransformComponent ("ghostTransform" + std::to_string (counter)));
		ghost->AddTransformComponent (pGhostTransform);
		pGhostTransform->SetGlobalPosition (ghostSpawner->Transform ()->GetGlobalPosition ());
		pGhostTransform->SetGlobalScale (Ogre::Vector3 (3.0f, 3.0f, 3.0f));

		std::shared_ptr<PlayerDataComponent> ghostData (new PlayerDataComponent ("ghostData", 2));
		ghost->AddComponent (ghostData);

		std::shared_ptr<EnemyAIComponent> ghostAI (new EnemyAIComponent ("ghostAI"));
		ghost->AddComponent (ghostAI);

		std::shared_ptr<EnemyAnimationComponent> ghostAnim (new EnemyAnimationComponent ("ghostAnim", "walk", "freeze", "death"));
		ghost->AddComponent (ghostAnim);

		ghost->Start ();
	}
}


void ManagerComponent::IncreaseKills ()
{
	++m_kills;
}
