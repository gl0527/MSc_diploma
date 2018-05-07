#include "ObjectManager.h"
#include "GameObject.h"
#include "Prefab/GameObjectCreator.h"
#include <iostream>


namespace Engine {

ObjectManager::~ObjectManager ()
{
	m_gameObjectMap.clear ();
}


ObjectManager& ObjectManager::GetInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (new ObjectManager); });
	return *s_pInstance.get ();
}


std::weak_ptr<GameObject> ObjectManager::CreateGameObject (const std::string& id)
{
	if (m_gameObjectMap.find (id) == m_gameObjectMap.end ()) {
		m_gameObjectMap[id] = std::shared_ptr<GameObject> (new GameObject (id));
		return m_gameObjectMap[id];
	}
	else {
		std::cout << "Gameobject creation failed: Gameobject with name \"" << id << "\" already existing.\n";
		return std::shared_ptr<GameObject> (nullptr);
	}
}


void ObjectManager::RemoveGameObject (const std::string& id)
{
	auto it = m_gameObjectMap.find (id);
	if (it != m_gameObjectMap.end ()) {
		// recursive call for the children
		const auto& removableChildren = it->second->GetChildrenNames ();
		for (auto childIt = removableChildren.begin (); childIt != removableChildren.end (); ++childIt)
			RemoveGameObject (*childIt);

		// erasing of the current element
		it->second->Destroy ();
		m_gameObjectMap.erase (it);
	}
}


void ObjectManager::MarkGameObjectForDelete (const std::string& id)
{
	m_removableGameObjectNames.push_back (id);
}


void ObjectManager::AddGameObjectCreator (std::shared_ptr<Prefab::GameObjectCreator> prefab)
{
	m_prefabMap[prefab->GetName ()] = prefab;
}


bool ObjectManager::GetGameObjectCreator (const std::string& name, std::shared_ptr<Prefab::GameObjectCreator>& outCreator)
{
	if (m_prefabMap.find (name) != m_prefabMap.end ()) {
		outCreator = m_prefabMap.at (name);

		return true;
	}

	return false;
}


void ObjectManager::Start ()
{
	for (auto it = m_gameObjectMap.begin (), itEnd = m_gameObjectMap.end (); it != itEnd; ++it)
		it->second->Start ();
}


void ObjectManager::PreUpdate (float t, float dt)
{
	for (auto it = m_gameObjectMap.begin (), itEnd = m_gameObjectMap.end (); it != itEnd; ++it)
		it->second->PreUpdate (t, dt);
}


void ObjectManager::Update (float t, float dt)
{
	for (auto it = m_gameObjectMap.begin (), itEnd = m_gameObjectMap.end (); it != itEnd; ++it)
		it->second->Update (t, dt);
}


void ObjectManager::PostUpdate (float t, float dt)
{
	for (auto it = m_gameObjectMap.begin (), itEnd = m_gameObjectMap.end (); it != itEnd; ++it)
		it->second->PostUpdate (t, dt);
}


void ObjectManager::RemoveMarkedGameObjects ()
{
	for (const std::string& gameObjectName : m_removableGameObjectNames)
		RemoveGameObject (gameObjectName);

	m_removableGameObjectNames.clear ();
}


void ObjectManager::Destroy ()
{
	for (auto it = m_gameObjectMap.begin (), itEnd = m_gameObjectMap.end (); it != itEnd; ++it)
		it->second->Destroy ();
}


std::weak_ptr<GameObject> ObjectManager::GetGameObjectByName (const std::string& objName) const
{
	if (m_gameObjectMap.find (objName) != m_gameObjectMap.end ())
		return m_gameObjectMap.at (objName);
	else {
		std::cout << "Gameobject with name \"" << objName << "\" not found.\n";
		return std::shared_ptr<GameObject> (nullptr);
	}
}

}	// namespace Engine
