#include "XML/LightProcessor.h"
#include "XML/XMLParser.h"

#include "LightCreator.h"
#include "ObjectManager.h"

#include <iostream>


namespace Engine {
namespace XML {

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

	if (strcmp (type, "point") == 0)
		lightType = Ogre::Light::LT_POINT;
	else if (strcmp (type, "directional") == 0)
		lightType = Ogre::Light::LT_DIRECTIONAL;
	else if (strcmp (type, "spot") == 0)
		lightType = Ogre::Light::LT_SPOTLIGHT;

	LightCreator lightCreator;

	auto& initData = lightCreator.GetInitData ();
	initData.name = name;
	initData.type = lightType;

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
			initData.diffuseColor = diffCol;
		}
		else if (childName == "specularcolor") {
			Ogre::ColourValue specCol;
			try {
				XMLParser::ParseFloat3_RGB (child, specCol);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			initData.specularColor = specCol;
		}
		else if (childName == "intensity") {
			float intensity;
			try {
				XMLParser::ParsePrimitive (child, "value", &intensity);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			initData.intensity = intensity;
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
			initData.range = range;
			initData.constantAttenuation = constant;
			initData.linearAttenuation = linear;
			initData.quadricAttenuation = quadric;
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
			initData.innerAngle = inner;
			initData.outerAngle = outer;
		}
	}

	std::string parentName;
	if (GetParentName (elem, parentName)) {
		auto object = ObjectManager::GetSingletonInstance ().GetGameObjectByName (parentName).lock ();

		lightCreator.Create (object.get ());
		lightCreator.ApplyInitData ();
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
