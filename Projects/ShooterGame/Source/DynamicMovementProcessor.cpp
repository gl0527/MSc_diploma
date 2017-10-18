#include "DynamicMovementProcessor.h"
#include "DynamicMovementComponent.h"
#include "XMLParser.h"


bool DynamicMovementProcessor::ProcessXMLTag(TiXmlElement* elem)
{
	const char* name;
	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	std::shared_ptr<DynamicMovementComponent> comp(new DynamicMovementComponent(name));

	AddToParentObject (elem, comp);

	foreach_child(elem)
	{
		std::string childName(child->Value());

		if (childName == "speed")
		{
			float speed;
			
			try {
				XMLParser::ParsePrimitive (child, "value", &speed);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setMoveSpeed(speed);
		}
	}

	return true;
}
