#include "XML/CameraProcessor.h"
#include "CameraComponent.h"
#include "XML/XMLParser.h"

#include "Prefab/GameObjectCreator.h"
#include "Prefab/GenericPrefab.h"
#include "ObjectManager.h"
#include "GameObject.h"

#include "Ogre.h"


namespace Engine {
namespace XML {

bool CameraProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name;
	int zOrder;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
		XMLParser::ParsePrimitive (elem, "zOrder", &zOrder);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	using CameraPrefab = Prefab::GenericPrefab<CameraComponent, CameraComponent::Descriptor>;

	CameraPrefab camPrefab;
	CameraComponent::Descriptor desc;

	desc.name = name;
	desc.zOrder = zOrder;

	foreach_child (elem)
	{
		std::string childName (child->Value ());

		if (childName == "frustum") {
			float nearPlane, farPlane;

			try {
				XMLParser::ParsePrimitive (child, "nearPlane", &nearPlane);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			try {
				XMLParser::ParsePrimitive (child, "farPlane", &farPlane);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.nearClip = nearPlane;
			desc.farClip = farPlane;
		}
		else if (childName == "lookat") {
			Ogre::Vector3 lookat;

			try {
				XMLParser::ParseFloat3_XYZ (child, lookat);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.lookat = lookat;
		}
		else if (childName == "renderdist") {
			float renderDist;

			try {
				XMLParser::ParsePrimitive (child, "dist", &renderDist);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.renderDist = renderDist;
		}
	}
	camPrefab.SetDescriptor (desc);

	std::string parentTag (elem->Parent ()->Value ());

	if (parentTag == std::string ("gameobject")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			auto object = ObjectManager::GetInstance ().GetGameObjectByName (parentName).lock ();

			camPrefab.Create ();
			camPrefab.Attach (object.get ());
			camPrefab.ApplyDescriptor ();
		}
	} else if (parentTag == std::string ("prefab")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			std::shared_ptr<Prefab::GameObjectCreator> prefab;

			if (ObjectManager::GetInstance ().GetGameObjectCreator (parentName, prefab))
				prefab->AddComponentCreator (std::make_shared<CameraPrefab> (camPrefab));
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
