#include "XML/MeshProcessor.h"
#include "MeshComponent.h"
#include "XML/XMLParser.h"

#include "Prefab/GameObjectCreator.h"
#include "Prefab/GenericPrefab.h"
#include "ObjectManager.h"
#include <iostream>


namespace Engine {
namespace XML {

bool MeshProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* entityName;
	const char* meshName;

	try {
		XMLParser::ParsePrimitive (elem, "name", &entityName);
		XMLParser::ParsePrimitive (elem, "mesh", &meshName);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	using MeshPrefab = Prefab::GenericPrefab<MeshComponent, MeshComponent::Descriptor>;

	MeshPrefab meshPrefab;
	MeshComponent::Descriptor desc;

	desc.entityName = entityName;
	desc.meshFileName = meshName;
	
	foreach_child (elem)
	{
		std::string childName (child->Value ());

		if (childName == "material") {
			const char* material;

			try {
				XMLParser::ParsePrimitive (child, "name", &material);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.materialName = material;
		}
		else if (childName == "shadows") {
			bool castShadows;

			try {
				XMLParser::ParsePrimitive (child, "value", &castShadows);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
		}
	}
	meshPrefab.SetDescriptor (desc);

	std::string parentTag (elem->Parent ()->Value ());

	if (parentTag == std::string ("gameobject")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			auto object = ObjectManager::GetInstance ().GetGameObjectByName (parentName).lock ();

			meshPrefab.Create ();
			meshPrefab.Attach (object.get ());
			meshPrefab.ApplyDescriptor ();
		}
	}
	else if (parentTag == std::string ("prefab")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			std::shared_ptr<Prefab::GameObjectCreator> prefab;

			if (ObjectManager::GetInstance ().GetGameObjectCreator (parentName, prefab))
				prefab->AddComponentCreator (std::make_shared<MeshPrefab> (meshPrefab));
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
