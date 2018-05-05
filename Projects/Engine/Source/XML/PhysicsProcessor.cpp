#include "XML/PhysicsProcessor.h"
#include "PhysicsComponent.h"
#include "XML/XMLParser.h"
#include "PhysicsMaterial.h"
#include "Prefab/GameObjectCreator.h"
#include "Prefab/GenericPrefab.h"
#include "ObjectManager.h"


namespace Engine {
namespace XML {
namespace {

bool ProcessCollisionShape (PhysicsComponent::Descriptor& phyDesc, TiXmlElement* child)
{
	PhysicsComponent::ShapeDescriptor shapeDesc;

	const char* shapeTypeCStr;
	try {
		XMLParser::ParsePrimitive (child, "type", &shapeTypeCStr);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}
	shapeDesc.shapeType = std::string (shapeTypeCStr);

	if (shapeDesc.shapeType == "box") {
		Ogre::Vector3 size;
		try {
			XMLParser::ParseFloat3_XYZ (child, size);
		} catch (const std::runtime_error& re) {
			std::cout << re.what () << std::endl;

			return false;
		}
		shapeDesc.shapeDimensions[0] = size.x;
		shapeDesc.shapeDimensions[1] = size.y;
		shapeDesc.shapeDimensions[2] = size.z;
	} else if (shapeDesc.shapeType == "staticplane") {
		float nx, ny, nz, d;
		try {
			XMLParser::ParsePrimitive (child, "nx", &nx);
			XMLParser::ParsePrimitive (child, "ny", &ny);
			XMLParser::ParsePrimitive (child, "nz", &nz);
			XMLParser::ParsePrimitive (child, "d", &d);
		} catch (const std::runtime_error& re) {
			std::cout << re.what () << std::endl;

			return false;
		}
		shapeDesc.shapeDimensions[0] = nx;
		shapeDesc.shapeDimensions[1] = ny;
		shapeDesc.shapeDimensions[2] = nz;
		shapeDesc.shapeDimensions[3] = d;
	} else if (shapeDesc.shapeType == "capsule") {
		float radius, height;
		try {
			XMLParser::ParsePrimitive (child, "r", &radius);
			XMLParser::ParsePrimitive (child, "h", &height);
		} catch (const std::runtime_error& re) {
			std::cout << re.what () << std::endl;

			return false;
		}
		shapeDesc.shapeDimensions[0] = radius;
		shapeDesc.shapeDimensions[1] = height;
	} else if (shapeDesc.shapeType == "sphere") {
		float radius;
		try {
			XMLParser::ParsePrimitive (child, "r", &radius);
		} catch (const std::runtime_error& re) {
			std::cout << re.what () << std::endl;

			return false;
		}
		shapeDesc.shapeDimensions[0] = radius;
	}

	Ogre::Vector3 shapePos = Ogre::Vector3::ZERO;
	Ogre::Quaternion shapeRot = Ogre::Quaternion::IDENTITY;

	for (TiXmlElement* child2 = child->FirstChildElement (); child2 != nullptr; child2 = child2->NextSiblingElement ()) {
		std::string childName2 (child2->Value ());

		if (childName2 == "position") {
			try {
				XMLParser::ParseFloat3_XYZ (child2, shapePos);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
		} else if (childName2 == "rotation") {
			try {
				XMLParser::ParseFloat4_WXYZ (child2, shapeRot);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
		}
	}
	shapeDesc.shapePos = shapePos;
	shapeDesc.shapeRot = shapeRot;
	phyDesc.shapeDescriptors.push_back (shapeDesc);

	return true;
}

}	// namespace


bool PhysicsProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name;
	float mass;
	const char* typeName;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
		XMLParser::ParsePrimitive (elem, "mass", &mass);
		XMLParser::ParsePrimitive (elem, "type", &typeName);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	using PhysicsPrefab = Prefab::GenericPrefab<PhysicsComponent, PhysicsComponent::Descriptor>;

	PhysicsPrefab phyPrefab;
	PhysicsComponent::Descriptor phyDesc;

	phyDesc.name = name;
	phyDesc.mass = mass;
	phyDesc.rigidBodyType = typeName;

	for (TiXmlElement* child = elem->FirstChildElement (); child != nullptr; child = child->NextSiblingElement ()) {
		std::string childName (child->Value ());

		if (childName == "shape") {
			ProcessCollisionShape (phyDesc, child);
		} else if (childName == "material") {
			float friction, linDamping, angDamping, bounciness;

			try {
				XMLParser::ParsePrimitive (child, "friction", &friction);
				XMLParser::ParsePrimitive (child, "lin_damping", &linDamping);
				XMLParser::ParsePrimitive (child, "ang_damping", &angDamping);
				XMLParser::ParsePrimitive (child, "bounciness", &bounciness);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			phyDesc.materialDescriptor.friction = friction;
			phyDesc.materialDescriptor.linearDamping = linDamping;
			phyDesc.materialDescriptor.angularDamping = angDamping;
			phyDesc.materialDescriptor.bounciness = bounciness;
		} else if (childName == "angularfactor") {
			Ogre::Vector3 angularfactor;

			try {
				XMLParser::ParseFloat3_XYZ (child, angularfactor);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			phyDesc.angularFactor = angularfactor;
		} else if (childName == "trigger") {
			bool isTrigger;

			try {
				XMLParser::ParsePrimitive (child, "value", &isTrigger);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			phyDesc.isTrigger = isTrigger;
		}
	}

	phyPrefab.SetDescriptor (phyDesc);

	std::string parentTag (elem->Parent ()->Value ());

	if (parentTag == std::string ("gameobject")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			auto object = ObjectManager::GetInstance ().GetGameObjectByName (parentName).lock ();

			phyPrefab.Create ();
			phyPrefab.Attach (object.get ());
			phyPrefab.ApplyDescriptor ();
		}
	} else if (parentTag == std::string ("prefab")) {
		std::string parentName;
		if (GetParentName (elem, parentName)) {
			std::shared_ptr<Prefab::GameObjectCreator> prefab;

			if (ObjectManager::GetInstance ().GetGameObjectCreator (parentName, prefab))
				prefab->AddComponentCreator (std::make_shared<PhysicsPrefab> (phyPrefab));
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
