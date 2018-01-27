#include "Prefab/GameObjectCreator.h"
#include "Prefab/IPrefab.h"
#include "ObjectManager.h"


namespace Engine {
namespace Prefab {

GameObjectCreator::GameObjectCreator (const std::string& name) : m_name (name)
{
}


void GameObjectCreator::AddComponentCreator (std::shared_ptr<Prefab::IPrefab> compCreator)
{
	m_compCreatorVec.push_back (compCreator);
}


void GameObjectCreator::Instantiate (const std::string& name) const
{
	if (auto spGameObj = ObjectManager::GetInstance ().CreateGameObject (name).lock ()) {
		for (auto& compCreator : m_compCreatorVec) {
			compCreator->Create ();
			compCreator->Attach (spGameObj.get ());
			compCreator->ApplyDescriptor ();
		}
	}
}


const std::string& GameObjectCreator::GetName () const
{
	return m_name;
}

}	// namespace Prefab
}	// namespace Engine
