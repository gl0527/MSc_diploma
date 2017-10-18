#include "LightProcessor.h"
#include "LightComponent.h"
#include "XMLParser.h"


namespace Engine
{

bool LightProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name = nullptr;
	const char* type = nullptr;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
		XMLParser::ParsePrimitive (elem, "type", &type);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	Ogre::Light::LightTypes lightType;

	if (type == "point")
		lightType = Ogre::Light::LT_POINT;
	else if (type == "directional")
		lightType = Ogre::Light::LT_DIRECTIONAL;
	else if (type == "spot")
		lightType = Ogre::Light::LT_SPOTLIGHT;

	std::shared_ptr<LightComponent> comp (new LightComponent (name, lightType));

	AddToParentObject (elem, comp);

	foreach_child (elem)
	{
		std::string childName (child->Value ());

		if (childName == "diffusecolor") {
			Ogre::ColourValue diffCol;

			try {
				XMLParser::ParseFloat3_RGB (child, diffCol);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setDiffuseColor (diffCol);
		}
		else if (childName == "specularcolor") {
			Ogre::ColourValue specCol;

			try {
				XMLParser::ParseFloat3_RGB (child, specCol);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setSpecularColor (specCol);
		}
		else if (childName == "intensity") {
			float intensity;

			try {
				XMLParser::ParsePrimitive (child, "value", &intensity);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setIntensity (intensity);
		}
		else if (childName == "attenuation") {
			float range, constant, linear, quadric;

			try {
				XMLParser::ParsePrimitive (child, "range", &range);
				XMLParser::ParsePrimitive (child, "constant", &constant);
				XMLParser::ParsePrimitive (child, "linear", &linear);
				XMLParser::ParsePrimitive (child, "quadric", &quadric);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setAttenuation (range, constant, linear, quadric);
		}
		else if (childName == "angle") {
			float inner, outer;

			try {
				XMLParser::ParsePrimitive (child, "inner", &inner);
				XMLParser::ParsePrimitive (child, "outer", &outer);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setSpotRange (Ogre::Degree (inner), Ogre::Degree (outer));
		}
	}

	return true;
}

}	// namespace Engine
