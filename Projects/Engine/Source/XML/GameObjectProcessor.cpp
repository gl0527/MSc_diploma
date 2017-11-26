#include <iostream>

#include "XML/GameObjectProcessor.h"
#include "XML/XMLParser.h"
#include "ObjectManager.h"
#include "GameObject.h"


namespace Engine {
namespace XML {

bool GameObjectProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	const auto& object = ObjectManager::GetSingletonInstance ().CreateGameObject (name);

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

	return true;
}

}	// namespace XML
}	// namespace Engine
