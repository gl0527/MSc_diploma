#include "XML/TagProcessor.h"

#include <iostream>

#include "GameObject.h"
#include "ObjectManager.h"
#include "XML/XMLParser.h"


namespace Engine {
namespace XML {

TagProcessor::TagProcessor (const std::string& tagName) : m_tagName (tagName)
{
	XMLParser::GetInstance ().AddXMLProcessor (this);
}


inline const std::string& TagProcessor::GetTagName () const
{
	return m_tagName;
}


void TagProcessor::AddToParentObject (TiXmlElement* elem, const std::shared_ptr<Component>& component)
{
	const char* objectName = nullptr;

	try {
		XMLParser::ParsePrimitive ((TiXmlElement*)elem->Parent (), "name", &objectName);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return;
	}

	const auto& object = ObjectManager::GetInstance ().GetGameObjectByName (objectName);

	if (auto obj = object.lock ())
		obj->AddComponent (component);
}


bool TagProcessor::GetParentName (TiXmlElement* elem, std::string& outName)
{
	const char* objectName = nullptr;

	try {
		XMLParser::ParsePrimitive ((TiXmlElement*)elem->Parent (), "name", &objectName);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}
	outName = std::string (objectName);

	return true;
}

}	// namespace XML
}	// namespace Engine
