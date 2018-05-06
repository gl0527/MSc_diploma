#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "SingletonBase.h"
#include <map>
#include <vector>
#include <memory>
#include "stdafx.h"


namespace Engine {

// ----------------------------- forward declaration-----------------------------

namespace Prefab {

class GameObjectCreator;

}

class GameObject;

// ============================= class ObjectManager =============================

class ObjectManager final : public SingletonBase<ObjectManager>
{
public:
	static DLL_EXPORT ObjectManager&		GetInstance ();

	DLL_EXPORT std::weak_ptr<GameObject>	CreateGameObject (const std::string& id);
	DLL_EXPORT void							MarkGameObjectForDelete (const std::string& id);

	void									AddGameObjectCreator (std::shared_ptr<Prefab::GameObjectCreator> prefab);
	DLL_EXPORT bool							GetGameObjectCreator (const std::string& name, std::shared_ptr<Prefab::GameObjectCreator>& outCreator);

	void									Start ();
	void									PreUpdate (float t, float dt);
	void									Update (float t, float dt);
	void									PostUpdate (float t, float dt);
	void									RemoveMarkedGameObjects ();
	void									Destroy ();

	DLL_EXPORT std::weak_ptr<GameObject>	GetGameObjectByName (const std::string& objName) const;

private:
	friend class SingletonBase<ObjectManager>;

	using GameObjectMap = std::map<std::string, std::shared_ptr<GameObject>>;
	using PrefabMap		= std::map<const std::string, std::shared_ptr<Prefab::GameObjectCreator>>;

	GameObjectMap				m_gameObjectMap;
	PrefabMap					m_prefabMap;
	std::vector<std::string>	m_removableGameObjectNames;

	ObjectManager () = default;
	~ObjectManager ();

	void RemoveGameObject (const std::string& id);
};

}	// namespace Engine

#endif	// #ifndef OBJECT_MANAGER_H
