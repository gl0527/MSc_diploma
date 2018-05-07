#include "XML/EnvironmentDataProcessor.h"
#include "XML/XMLParser.h"
#include "RenderSystem.h"
#include "OgreSceneManager.h"
#include "OgreColourValue.h"


namespace Engine {
namespace XML {

bool EnvironmentDataProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	Ogre::SceneManager* sceneMgr = RenderSystem::GetInstance ().GetSceneManager ();

	foreach_child (elem)
	{
		std::string childName (child->Value ());

		if (childName == "ambientlight") {
			float r, g, b;
			try {
				XMLParser::ParsePrimitive (child, "r", &r);
				XMLParser::ParsePrimitive (child, "g", &g);
				XMLParser::ParsePrimitive (child, "b", &b);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			sceneMgr->setAmbientLight (Ogre::ColourValue (r, g, b, 1.0f));
		} else if (childName == "skybox") {
			const char* materialName;
			float distance;
			try {
				XMLParser::ParsePrimitive (child, "material", &materialName);
				XMLParser::ParsePrimitive (child, "distance", &distance);
			} catch (const std::runtime_error& re) {
				std::cout << re.what () << std::endl;

				return false;
			}
			sceneMgr->setSkyBox (true, materialName, distance, true);
		}
	}

	return true;
}

}	// namespace XML
}	// namespace Engine
