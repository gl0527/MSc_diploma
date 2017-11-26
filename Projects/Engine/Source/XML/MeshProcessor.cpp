#include "XML/MeshProcessor.h"
#include "MeshComponent.h"
#include "XML/XMLParser.h"
#include "Ogre.h"


namespace Engine {
namespace XML {

bool MeshProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* entityName;
	const char* meshName;

	try {
		XMLParser::ParsePrimitive (elem, "name", &entityName);
		XMLParser::ParsePrimitive (elem, "mesh", &meshName);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}

	std::shared_ptr<MeshComponent> comp (new MeshComponent (entityName, meshName));

	AddToParentObject (elem, comp);

	foreach_child (elem)
	{
		std::string childName (child->Value ());

		if (childName == "material") {
			const char* material;

			try {
				XMLParser::ParsePrimitive (child, "name", &material);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->setMaterial (material);
		}
		else if (childName == "shadows") {
			bool castShadows;

			try {
				XMLParser::ParsePrimitive (child, "value", &castShadows);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}

			comp->SetCastShadows (castShadows);
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
