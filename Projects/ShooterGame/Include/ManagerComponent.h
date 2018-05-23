#ifndef MANAGER_COMPONENT_H
#define MANAGER_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"

using namespace Engine;

// ----------------------------- forward declaration -----------------------------

namespace Engine {

class GameObject;

namespace Prefab {

class GameObjectCreator;

}	// namespace Prefab
}	// namespace Engine

class HUDComponent;
class PlayerDataComponent;
class WeaponComponent;

// ============================ class ManagerComponent ===========================

class ManagerComponent : public Component
{
public:
	explicit	ManagerComponent (const std::string& name, unsigned int spawningPeriod);

	void		Start () override;
	void		PreUpdate (float t, float dt) override;
	void		IncreaseKills ();

private:
	const unsigned int							m_SpawningPeriod;

	unsigned int								m_kills;
	unsigned int								m_round;
	std::shared_ptr<Prefab::GameObjectCreator>	m_pPacManPrefab;
	std::shared_ptr<Prefab::GameObjectCreator>	m_pGhostPrefab;
	std::shared_ptr<GameObject>					m_pPlayer;
	std::shared_ptr<PlayerDataComponent>		m_pPlayerData;
	std::shared_ptr<WeaponComponent>			m_pPlayerWeapon;
	std::shared_ptr<HUDComponent>				m_pOwnerHUD;

	void		CreatePacMan (unsigned int counter);
	void		CreateGhost (unsigned int counter);
};

#endif	// #ifndef MANAGER_COMPONENT_H
