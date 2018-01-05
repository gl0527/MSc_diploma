#include "XML/LightProcessor.h"
#include "XML/XMLParser.h"

#include "Prefab/GameObjectCreator.h"
#include "Prefab/GenericPrefab.h"

#include "LightComponent.h"
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
	else
		lightType = Ogre::Light::LT_POINT;

	using LightPrefab = Prefab::GenericPrefab<LightComponent, LightComponent::Descriptor>;

	LightPrefab lightPrefab;
	LightComponent::Descriptor desc;

	desc.name = name;
	desc.type = lightType;

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
			desc.diffuseColor = diffCol;
		}
		else if (childName == "specularcolor") {
			Ogre::ColourValue specCol;
			try {
				XMLParser::ParseFloat3_RGB (child, specCol);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.specularColor = specCol;
		}
		else if (childName == "intensity") {
			float intensity;
			try {
				XMLParser::ParsePrimitive (child, "value", &intensity);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			desc.intensity = intensity;
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
			desc.range = range;
			desc.constantAttenuation = constant;
			desc.linearAttenuation = linear;
			desc.quadricAttenuation = quadric;
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
			desc.innerAngle = inner;
			desc.outerAngle = outer;
		}
	}
	lightPrefab.SetDescriptor (desc);

	std::string parentTag (elem->Parent ()->Value ());

	if (parentTag == std::string ("gameobject")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			auto object = ObjectManager::GetInstance ().GetGameObjectByName (parentName).lock ();

			lightPrefab.Create ();
			lightPrefab.Attach(object.get ());
			lightPrefab.ApplyDescriptor ();
		}
	} else if (parentTag == std::string ("prefab")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			std::shared_ptr<Prefab::GameObjectCreator> prefab;

			if (ObjectManager::GetInstance ().GetGameObjectCreator (parentName, prefab))
				prefab->AddComponentCreator (std::make_shared<LightPrefab> (lightPrefab));
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
