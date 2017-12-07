#include "XML/PrefabProcessor.h"

#include "XML/XMLParser.h"
#include "ObjectManager.h"
#include "Prefab/GameObjectCreator.h"

#include <iostream>


namespace Engine {
namespace XML {

bool PrefabProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}
	
	std::shared_ptr<Prefab::GameObjectCreator> prefab (new Prefab::GameObjectCreator (name));
	ObjectManager::GetInstance ().AddGameObjectCreator (prefab);

	return true;
}

}	// namespace XML
}	// namespace Engine