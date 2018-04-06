#include "XML/AudioProcessor.h"

#include "AudioSourceComponent.h"

#include "XML/XMLParser.h"

#include "Prefab/GameObjectCreator.h"
#include "Prefab/GenericPrefab.h"
#include "ObjectManager.h"

#include <iostream>


namespace Engine {
namespace XML {

bool AudioProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* compName, *typeName;

	try {
		XMLParser::ParsePrimitive (elem, "name", &compName);
		XMLParser::ParsePrimitive (elem, "type", &typeName);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	AudioSourceComponent::AudioType type;

	if (strcmp (typeName, "SoundEffect") == 0) {
		type = AudioSourceComponent::SoundEffect;
	} else if (strcmp (typeName, "Music") == 0) {
		type = AudioSourceComponent::Music;
	} else if (strcmp (typeName, "Ambient") == 0) {
		type = AudioSourceComponent::Ambient;
	}

	using AudioPrefab = Prefab::GenericPrefab<AudioSourceComponent, AudioSourceComponent::Descriptor>;

	AudioPrefab audioPrefab;
	AudioSourceComponent::Descriptor desc;

	desc.name = compName;
	desc.audioType = type;

	foreach_child (elem)
	{
		std::string childTag (child->Value ());

		if (childTag == "volume") {
			float volume;

			try {
				XMLParser::ParsePrimitive (child, "value", &volume);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.volume = volume;
		} else if (childTag == "speed") {
			float speed;

			try {
				XMLParser::ParsePrimitive (child, "value", &speed);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.speed = speed;
		} else if (childTag == "loop") {
			bool loop;

			try {
				XMLParser::ParsePrimitive (child, "value", &loop);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.isLooping = loop;
		} else if (childTag == "buffer") {
			const char* bufferName;

			try {
				XMLParser::ParsePrimitive (child, "name", &bufferName);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.buffers.push_back (bufferName);
		} else if (childTag == "refdist") {
			float refDist;

			try {
				XMLParser::ParsePrimitive (child, "value", &refDist);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			desc.maxDistWithFullGain = refDist;
		} else if (childTag == "maxdist") {
			float maxDist;

			try {
				XMLParser::ParsePrimitive (child, "value", &maxDist);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.minDistWithZeroGain = maxDist;
		} else if (childTag == "randombufferselection") {
			bool isRandomSelectedBuffers;

			try {
				XMLParser::ParsePrimitive (child, "value", &isRandomSelectedBuffers);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.isBufferRandomlySelected = isRandomSelectedBuffers;
		}
	}

	audioPrefab.SetDescriptor (desc);

	std::string parentTag (elem->Parent ()->Value ());

	if (parentTag == std::string ("gameobject")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			auto object = ObjectManager::GetInstance ().GetGameObjectByName (parentName).lock ();

			audioPrefab.Create ();
			audioPrefab.Attach (object.get ());
			audioPrefab.ApplyDescriptor ();
		}
	} else if (parentTag == std::string ("prefab")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			std::shared_ptr <Prefab::GameObjectCreator> prefab;

			if (ObjectManager::GetInstance ().GetGameObjectCreator (parentName, prefab))
				prefab->AddComponentCreator (std::make_shared<AudioPrefab> (audioPrefab));
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
