#include "DynamicMovementProcessor.h"
#include "DynamicMovementComponent.h"
#include "XML/XMLParser.h"
#include <iostream>


bool DynamicMovementProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name;
	try {
		XML::XMLParser::ParsePrimitive (elem, "name", &name);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	std::shared_ptr<DynamicMovementComponent> comp (new DynamicMovementComponent (name));

	AddToParentObject (elem, comp);

	foreach_child (elem)
	{
		std::string childName (child->Value ());

		if (childName == "movespeed") {
			float moveSpeed;
			try {
				XML::XMLParser::ParsePrimitive (child, "value", &moveSpeed);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			comp->SetMoveSpeed (moveSpeed);
		} else if (childName == "turnspeed") {
			float turnSpeed;
			try {
				XML::XMLParser::ParsePrimitive (child, "value", &turnSpeed);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			comp->SetTurnSpeed (turnSpeed);
		}
	}

	return true;
}
