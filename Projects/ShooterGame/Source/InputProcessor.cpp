#include <iostream>

#include "InputProcessor.h"
#include "InputComponent.h"
#include "XML/XMLParser.h"


bool InputProcessor::ProcessXMLTag(TiXmlElement* elem)
{
	const char* name;
	
	try {
		XML::XMLParser::ParsePrimitive (elem, "name", &name);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}
	
	std::shared_ptr<InputComponent> comp(new InputComponent(name));

	AddToParentObject(elem, comp);

	foreach_child(elem)
	{
		std::string childName(child->Value());

		if (childName == "movespeed")
		{
			float value;
			
			try {
				XML::XMLParser::ParsePrimitive (child, "value", &value);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			
			comp->setMoveSpeed(value);
		}
		else if (childName == "turnspeed")
		{
			float value;
			
			try {
				XML::XMLParser::ParsePrimitive (child, "value", &value);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setTurnSpeed(value);
		}
		else if (childName == "mousesens")
		{
			float value;
			
			try {
				XML::XMLParser::ParsePrimitive (child, "value", &value);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setMouseSens(value);
		}
		else if (childName == "maxdelay")
		{
			int value;

			try {
				XML::XMLParser::ParsePrimitive (child, "value", &value);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setMaxDelay(value);
		}
	}

	return true;
}