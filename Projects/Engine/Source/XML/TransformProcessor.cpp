#include "XML/TransformProcessor.h"
#include "TransformComponent.h"

#include "GameObject.h"
#include "ObjectManager.h"
#include "XML/XMLParser.h"


namespace Engine {
namespace XML {

void TransformProcessor::AddToParentObject (TiXmlElement* elem, const std::shared_ptr<Component>& component)
{
	const char* objectName = nullptr;

	try {
		XMLParser::ParsePrimitive ((TiXmlElement*)elem->Parent (), "name", &objectName);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return;
	}

	const auto& object = ObjectManager::GetInstance ().GetGameObjectByName (objectName);

	if (auto obj = object.lock ()) {
		obj->AddTransformComponent (std::dynamic_pointer_cast<TransformComponent> (component));
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

	bool isPositionSet = false, isRotationSet = false, isScaleSet = false;

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

			comp->SetLocalPosition (position);
			isPositionSet = true;
		} else if (childName == "rotation") {
			Ogre::Quaternion rotation;

			try {
				XMLParser::ParseFloat4_WXYZ (child, rotation);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->SetLocalRotation (rotation);
			isRotationSet = true;
		} else if (childName == "scale") {
			Ogre::Vector3 scale;

			try {
				XMLParser::GetInstance ().ParseFloat3_XYZ (child, scale);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->SetLocalScale (scale);
			isScaleSet = true;
		}
	}

	if (!isPositionSet)
		comp->SetLocalPosition (Ogre::Vector3::ZERO);

	if (!isRotationSet)
		comp->SetLocalRotation (Ogre::Quaternion::IDENTITY);
	
	if (!isScaleSet)
		comp->SetLocalScale (Ogre::Vector3::UNIT_SCALE);

	return true;
}

}	// namespace XML
}	// namespace Engine
