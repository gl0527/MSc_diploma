#include "XML/PhysicsProcessor.h"
#include "PhysicsComponent.h"
#include "Ogre.h"
#include "XML/XMLParser.h"


namespace Engine {
namespace XML {

bool PhysicsProcessor::ProcessCollisionShape (PhysicsComponent* comp, TiXmlElement* child)
{
	const char* shapeTypeCStr;

	try {
		XMLParser::ParsePrimitive (child, "type", &shapeTypeCStr);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	std::string shapeType (shapeTypeCStr);

	btCollisionShape* collShape = nullptr;

	if (shapeType == "box") {
		Ogre::Vector3 size;

		try {
			XMLParser::ParseFloat3_XYZ (child, size);
		} catch (const std::runtime_error& re) {
			std::cout << re.what () << std::endl;

			return false;
		}

		collShape = new btBoxShape (btVector3 (size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
	}
	else if (shapeType == "staticplane") {
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

		collShape = new btStaticPlaneShape (btVector3 (nx, ny, nz), d);
	}
	else if (shapeType == "capsule") {
		float radius, height;

		try {
			XMLParser::ParsePrimitive (child, "r", &radius);
			XMLParser::ParsePrimitive (child, "h", &height);
		} catch (const std::runtime_error& re) {
			std::cout << re.what () << std::endl;

			return false;
		}

		collShape = new btCapsuleShape (radius, height);
	}
	else if (shapeType == "sphere") {
		float radius;

		try {
			XMLParser::ParsePrimitive (child, "r", &radius);
		} catch (const std::runtime_error& re) {
			std::cout << re.what () << std::endl;

			return false;
		}

		collShape = new btSphereShape (radius);
	}

	auto shapePos = Ogre::Vector3::ZERO;
	auto shapeRot = Ogre::Quaternion::IDENTITY;

	for (TiXmlElement* child2 = child->FirstChildElement (); child2 != nullptr; child2 = child2->NextSiblingElement ()) {
		std::string childName2 (child2->Value ());

		if (childName2 == "position") {
			try {
				XMLParser::ParseFloat3_XYZ (child2, shapePos);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
		}
		else if (childName2 == "rotation") {
			try {
				XMLParser::ParseFloat4_WXYZ (child2, shapeRot);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
		}
	}
	comp->AddCollisionShape (collShape, shapePos, shapeRot);

	return true;
}


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

	std::shared_ptr<PhysicsComponent> comp (new PhysicsComponent (name, mass));

	if (strcmp (typeName, "dynamic") == 0)
		comp->SetTypeToDynamic ();
	else if (strcmp (typeName, "kinematic") == 0)
		comp->SetTypeToKinematic ();
	else if (strcmp (typeName, "static") == 0)
		comp->SetTypeToStatic ();
	else
		return false;

	unsigned int shapeCount = 0;

	for (TiXmlElement* child = elem->FirstChildElement (); child != nullptr; child = child->NextSiblingElement ()) {
		std::string childName (child->Value ());

		if (childName == "shape") {
			shapeCount++;

			ProcessCollisionShape (comp.get (), child);

			if (shapeCount == 1)
				AddToParentObject (elem, comp);
		}
		else if (childName == "material") {
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

			PhysicsMaterial phyMat;

			phyMat.SetFriction (friction);
			phyMat.SetLinearDamping (linDamping);
			phyMat.SetAngularDamping (angDamping);
			phyMat.SetBounciness (bounciness);

			comp->SetPhysicsMaterial (phyMat);
		}
		else if (childName == "angularfactor") {
			Ogre::Vector3 angularfactor;

			try {
				XMLParser::ParseFloat3_XYZ (child, angularfactor);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->SetAngularFactor (angularfactor.x, angularfactor.y, angularfactor.z);
		}
		else if (childName == "trigger") {
			bool isTrigger;

			try {
				XMLParser::ParsePrimitive (child, "value", &isTrigger);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->SetTrigger (isTrigger);
		}
		else if (childName == "rotation") {
			bool enableRotation;

			try {
				XMLParser::ParsePrimitive (child, "enabled", &enableRotation);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			if (!enableRotation)
				comp->DisableRotationXYZ ();
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
