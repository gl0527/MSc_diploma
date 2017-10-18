#include "TransformProcessor.h"
#include "TransformComponent.h"

#include "GameObject.h"
#include "ObjectManager.h"
#include "XMLParser.h"


namespace Engine
{

void TransformProcessor::AddToParentObject (TiXmlElement* elem, const std::shared_ptr<Component>& component)
{
	const char* objectName = nullptr;

	try {
		XMLParser::ParsePrimitive ((TiXmlElement*)elem->Parent (), "name", &objectName);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return;
	}

	const auto& object = ObjectManager::GetSingletonInstance ().GetGameObjectByName (objectName);

	if (auto& obj = object.lock ()) {
		obj->removeComponent (obj->GetName ()); // a régi transform kitörlése
		obj->AddComponent (component);
	}
}


bool TransformProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name = nullptr;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	std::shared_ptr<TransformComponent> comp (new TransformComponent (name));

	AddToParentObject (elem, comp);

	foreach_child (elem)
	{
		std::string childName (child->Value ());

		if (childName == "position") {
			Ogre::Vector3 position;

			try {
				XMLParser::ParseFloat3_XYZ (child, position);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setPosition (position);
		}
		else if (childName == "rotation") {
			Ogre::Quaternion rotation;

			try {
				XMLParser::ParseFloat4_WXYZ (child, rotation);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setRotation (rotation);
		}
		else if (childName == "scale") {
			Ogre::Vector3 scale;

			try {
				XMLParser::GetInstance ().ParseFloat3_XYZ (child, scale);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setScale (scale);
		}
	}

	return true;
}

}	// namespace Engine
