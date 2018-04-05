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

	std::shared_ptr<AudioSourceComponent> audioComp (new AudioSourceComponent (compName, type));
	AddToParentObject (elem, audioComp);

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
			audioComp->SetVolume (volume);
		} else if (childTag == "speed") {
			float speed;

			try {
				XMLParser::ParsePrimitive (child, "value", &speed);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			audioComp->SetSpeed (speed);
		} else if (childTag == "loop") {
			bool loop;

			try {
				XMLParser::ParsePrimitive (child, "value", &loop);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			audioComp->SetLooping (loop);
		} else if (childTag == "buffer") {
			const char* bufferName;

			try {
				XMLParser::ParsePrimitive (child, "name", &bufferName);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			audioComp->AddBuffer (bufferName);
		} else if (childTag == "refdist") {
			float refDist;

			try {
				XMLParser::ParsePrimitive (child, "value", &refDist);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			audioComp->SetMaxDistanceWithFullGain (refDist);
		} else if (childTag == "maxdist") {
			float maxDist;

			try {
				XMLParser::ParsePrimitive (child, "value", &maxDist);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			audioComp->SetMinDistanceWithZeroGain (maxDist);
		} else if (childTag == "randombufferselection") {
			bool isRandomSelectedBuffers;

			try {
				XMLParser::ParsePrimitive (child, "value", &isRandomSelectedBuffers);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			audioComp->SetRandomBuffers (isRandomSelectedBuffers);
		}
	}

	return true;
}

}
}	// namespace Engine
