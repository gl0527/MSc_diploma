#include <iostream>

#include "AudioProcessor.h"
#include "AudioComponent.h"
#include "XMLParser.h"


namespace Engine
{

bool AudioProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* file;
	const char* listener;

	try {
		XMLParser::ParsePrimitive (elem, "file", &file);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	try {
		XMLParser::ParsePrimitive (elem, "listener", &listener);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	std::shared_ptr<AudioComponent> comp (new AudioComponent (file, listener));

	AddToParentObject (elem, comp);

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

			comp->setVolume (volume);
		}
		else if (childName == "speed") {
			float speed;

			try {
				XMLParser::ParsePrimitive (child, "value", &speed);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setSpeed (speed);
		}
		else if (childName == "loop") {
			bool loop;

			try {
				XMLParser::ParsePrimitive (child, "value", &loop);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setLooping (loop);
		}
	}
	return true;
}

}	// namespace Engine
