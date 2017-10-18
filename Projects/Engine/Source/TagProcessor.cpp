#include "TagProcessor.h"

#include <iostream>

#include "GameObject.h"
#include "ObjectManager.h"
#include "XMLParser.h"


namespace Engine
{

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

	const auto& object = ObjectManager::GetSingletonInstance ().GetGameObjectByName (objectName);

	if (auto& obj = object.lock ())
		obj->AddComponent (component);
}

}	// namespace Engine
