#include <iostream>

#include "XML/GameObjectProcessor.h"
#include "XML/XMLParser.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Prefab/GameObjectCreator.h"


namespace Engine {
namespace XML {

bool GameObjectProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name = nullptr;
	const char* prefabName = nullptr;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	XMLParser::ParsePrimitive (elem, "using", &prefabName, true);
	
	if (prefabName == nullptr) {
		const auto& object = ObjectManager::GetInstance ().CreateGameObject (name);

		if (auto obj = object.lock ()) {
			auto parent = (TiXmlElement*)elem->Parent ();
			std::string parentTag (parent->Value ());

			if (parentTag == "gameobject") {
				const char* parentName;

				try {
					XMLParser::ParsePrimitive (parent, "name", &parentName);
				} catch (const std::runtime_error& re) {
					std::cout << re.what () << std::endl;

					return false;
				}

				obj->setParent (parentName);
			}
		}
	} else {
		std::shared_ptr<Prefab::GameObjectCreator> pPrefab (nullptr);
		auto& objMgr = ObjectManager::GetInstance ();

		if (objMgr.GetGameObjectCreator (prefabName, pPrefab)) {
			pPrefab->Instantiate (name);
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
