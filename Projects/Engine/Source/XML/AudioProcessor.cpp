#include "XML/AudioProcessor.h"

#include "AudioComponent.h"

#include "XML/XMLParser.h"

#include "Prefab/GameObjectCreator.h"
#include "Prefab/GenericPrefab.h"
#include "ObjectManager.h"

#include <iostream>


namespace Engine {
namespace XML {

bool AudioProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* file;
	const char* listener;

	try {
		XMLParser::ParsePrimitive (elem, "file", &file);
		XMLParser::ParsePrimitive (elem, "listener", &listener);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	using AudioPrefab = Prefab::GenericPrefab<AudioComponent, AudioComponent::Descriptor>;

	AudioPrefab audioPrefab;
	AudioComponent::Descriptor desc;

	desc.fileName = file;
	desc.listenerName = listener;

	foreach_child (elem)
	{
		std::string childName (child->Value ());

		if (childName == "volume") {
			float volume;

			try {
				XMLParser::ParsePrimitive (child, "value", &volume);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.volume = volume;
		}
		else if (childName == "speed") {
			float speed;

			try {
				XMLParser::ParsePrimitive (child, "value", &speed);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.speed = speed;
		}
		else if (childName == "loop") {
			bool loop;

			try {
				XMLParser::ParsePrimitive (child, "value", &loop);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.loop = loop;
		}
	}

	audioPrefab.SetDescriptor(desc);

	std::string parentTag (elem->Parent ()->Value ());

	if (parentTag == std::string ("gameobject")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			auto object = ObjectManager::GetInstance ().GetGameObjectByName (parentName).lock ();

			audioPrefab.Create ();
			audioPrefab.Attach (object.get ());
			audioPrefab.ApplyDescriptor ();
		}
	}
	else if (parentTag == std::string ("prefab")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			std::shared_ptr<Prefab::GameObjectCreator> prefab;

			if (ObjectManager::GetInstance ().GetGameObjectCreator (parentName, prefab))
				prefab->AddComponentCreator (std::make_shared<AudioPrefab> (audioPrefab));
		}
	}

	return true;
}

}
}	// namespace Engine
