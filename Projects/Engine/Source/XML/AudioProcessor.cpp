#include <iostream>

#include "XML/AudioProcessor.h"
#include "AudioComponent.h"
#include "Prefab.h"
#include "XML/XMLParser.h"


namespace Engine {
namespace XML {

bool AudioProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* file;
	const char* listener;
	bool immediateCreation;

	try {
		XMLParser::ParsePrimitive (elem, "file", &file);
		XMLParser::ParsePrimitive (elem, "listener", &listener);
		XMLParser::ParsePrimitive (elem, "immediate_creation", &immediateCreation);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	/*Prefab<AudioComponent> audioFactory (immediateCreation);
	audioFactory.SetPrimaryAttributes (file, listener);
	//AddPrefabToParentObject (elem, audioFactory);

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
	audioFactory.SetSecondaryAttribute<0> (volume);
	}
	else if (childName == "speed") {
	float speed;

	try {
	XMLParser::ParsePrimitive (child, "value", &speed);
	} catch (const std::runtime_error& re) {
	std::cout << re.what () << std::endl;

	return false;
	}
	audioFactory.SetSecondaryAttribute<1> (speed);
	}
	else if (childName == "loop") {
	bool loop;

	try {
	XMLParser::ParsePrimitive (child, "value", &loop);
	} catch (const std::runtime_error& re) {
	std::cout << re.what () << std::endl;

	return false;
	}
	audioFactory.SetSecondaryAttribute<2> (loop);
	}
	}*/

	return true;
}

}
}	// namespace Engine
