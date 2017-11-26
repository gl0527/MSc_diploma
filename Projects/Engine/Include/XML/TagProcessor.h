#ifndef TAG_PROCESSOR_H
#define TAG_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include <string>
#include <memory>

// TODO ettol surgosen meg kellene szabadulni
#define foreach_child(elem) for (auto child = elem->FirstChildElement(); \
								child != nullptr; child = child->NextSiblingElement())

// ----------------------------- forward declaration -----------------------------

class TiXmlElement;


namespace Engine {

// ----------------------------- forward declaration -----------------------------

class Component;
class PrefabBase;

namespace XML {

// ============================= class TagProcessor =============================

class TagProcessor
{
public:
	explicit DLL_EXPORT TagProcessor (const std::string& tagName);
	virtual ~TagProcessor () {}

	virtual bool		ProcessXMLTag (TiXmlElement* elem) = 0;
	DLL_EXPORT const std::string&	GetTagName () const;

protected:
	std::string m_tagName;

	virtual DLL_EXPORT void AddToParentObject (TiXmlElement* elem, const std::shared_ptr<Component>& component);
	virtual DLL_EXPORT void AddPrefabToParentObject (TiXmlElement* elem, std::shared_ptr<PrefabBase> pPrefab);
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef TAG_PROCESSOR_H
