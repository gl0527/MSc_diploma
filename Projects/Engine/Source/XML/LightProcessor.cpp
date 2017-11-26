#include <iostream>

#include "LightComponent.h"
#include "XML/LightProcessor.h"
#include "XML/XMLParser.h"
#include "Prefab.h"


namespace Engine {
namespace XML {

bool LightProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name = nullptr;
	const char* type = nullptr;
	bool immediateCreation;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
		XMLParser::ParsePrimitive (elem, "type", &type);
		XMLParser::ParsePrimitive (elem, "immediate_creation", &immediateCreation);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	Ogre::Light::LightTypes lightType;

	if (strcmp (type, "point"))
		lightType = Ogre::Light::LT_POINT;
	else if (strcmp (type, "directional"))
		lightType = Ogre::Light::LT_DIRECTIONAL;
	else if (strcmp (type, "spot"))
		lightType = Ogre::Light::LT_SPOTLIGHT;

	std::shared_ptr<Prefab<LightComponent>> lightPrefab (new Prefab<LightComponent> (immediateCreation));

	lightPrefab->StorePrimaryParams (std::string (name), lightType);
	AddPrefabToParentObject (elem, lightPrefab);

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
			lightPrefab->StoreSecondaryParam<0> (diffCol);
		}
		else if (childName == "specularcolor") {
			Ogre::ColourValue specCol;
			try {
				XMLParser::ParseFloat3_RGB (child, specCol);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			lightPrefab->StoreSecondaryParam<1> (specCol);
		}
		else if (childName == "intensity") {
			float intensity;
			try {
				XMLParser::ParsePrimitive (child, "value", &intensity);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			lightPrefab->StoreSecondaryParam<2> (intensity);
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
			lightPrefab->StoreSecondaryParam<3> (range);
			lightPrefab->StoreSecondaryParam<4> (constant);
			lightPrefab->StoreSecondaryParam<5> (linear);
			lightPrefab->StoreSecondaryParam<6> (quadric);
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
			lightPrefab->StoreSecondaryParam<7> (inner);
			lightPrefab->StoreSecondaryParam<8> (outer);
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
