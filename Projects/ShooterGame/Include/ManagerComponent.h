#ifndef MANAGER_COMPONENT_H
#define MANAGER_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"

using namespace Engine;

// ----------------------------- forward declaration -----------------------------

namespace Engine {
namespace Prefab {

class GameObjectCreator;

}	// namespace Prefab
}	// namespace Engine

namespace Ogre {

class Vector3;

}	// namespace Ogre

// ============================ class ManagerComponent ===========================

class ManagerComponent : public Component
{
public:
	explicit	ManagerComponent (const std::string& name);

	void		Start () override;
	void		PreUpdate (float t, float dt) override;

	void		CreateBullet (unsigned int counter, const Ogre::Vector3& weaponWorldPos, const Ogre::Vector3& weaponWorldDir);

private:
	std::shared_ptr<Prefab::GameObjectCreator> m_pPacManPrefab;
	std::shared_ptr<Prefab::GameObjectCreator> m_pGhostPrefab;
	std::shared_ptr<Prefab::GameObjectCreator> m_pBulletPrefab;

	void		CreatePacMan (unsigned int counter);
	void		CreateGhost (unsigned int counter);
};

#endif	// #ifndef MANAGER_COMPONENT_H
