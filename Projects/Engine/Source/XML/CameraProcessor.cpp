#include "XML/CameraProcessor.h"
#include "CameraComponent.h"
#include "XML/XMLParser.h"
#include "Ogre.h"


namespace Engine {
namespace XML {

bool CameraProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name;
	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	int zOrder;
	try {
		XMLParser::ParsePrimitive (elem, "zOrder", &zOrder);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	std::shared_ptr<CameraComponent> comp (new CameraComponent (name, zOrder));

	AddToParentObject (elem, comp);

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

			comp->setNearClip (nearPlane);
			comp->setFarClip (farPlane);
		}
		else if (childName == "lookat") {
			Ogre::Vector3 lookat;

			try {
				XMLParser::ParseFloat3_XYZ (child, lookat);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setLookAt (lookat);
		}
		else if (childName == "renderdist") {
			float renderDist;

			try {
				XMLParser::ParsePrimitive (child, "dist", &renderDist);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setRenderDist (renderDist);
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
