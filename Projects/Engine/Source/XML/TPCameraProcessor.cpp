#include "XML/TPCameraProcessor.h"
#include "TPCameraComponent.h"
#include "XML/XMLParser.h"
#include "Ogre.h"


namespace Engine {
namespace XML {

bool TPCameraProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name = nullptr;
	int zDepth;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
		XMLParser::ParsePrimitive (elem, "zdepth", &zDepth);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	std::shared_ptr<TPCameraComponent> comp (new TPCameraComponent (name, zDepth));

	AddToParentObject (elem, comp);

	foreach_child (elem)
	{
		std::string childName (child->Value ());

		if (childName == "frustum") {
			float nearPlane, farPlane;

			try {
				XMLParser::ParsePrimitive (child, "near", &nearPlane);
				XMLParser::ParsePrimitive (child, "far", &farPlane);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->SetNearClip (nearPlane);
			comp->SetFarClip (farPlane);
		}
		else if (childName == "height") {
			float height;

			try {
				XMLParser::ParsePrimitive (child, "value", &height);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setCameraHeight (height);
		}
		else if (childName == "targetHeight") {
			float targetHeight;

			try {
				XMLParser::ParsePrimitive (child, "value", &targetHeight);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setTargetHeight (targetHeight);
		}
		else if (childName == "distfromtarget") {
			float distFromTarget;

			try {
				XMLParser::ParsePrimitive (child, "value", &distFromTarget);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setCameraDist (distFromTarget);
		}
		else if (childName == "motionblend") {
			float motionBlend;

			try {
				XMLParser::ParsePrimitive (child, "value", &motionBlend);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setMotionBlend (motionBlend);
		}
		else if (childName == "fixed") {
			bool isFixed;

			try {
				XMLParser::ParsePrimitive (child, "value", &isFixed);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setFixed (isFixed);
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
