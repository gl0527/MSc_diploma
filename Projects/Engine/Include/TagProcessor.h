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


namespace Engine
{

// ----------------------------- forward declaration -----------------------------

class Component;

// ============================= class TagProcessor =============================

class DLL_EXPORT TagProcessor
{
public:
	explicit TagProcessor (const std::string& tagName);

	virtual bool		ProcessXMLTag (TiXmlElement* elem) = 0;
	const std::string&	GetTagName () const;

protected:
	std::string m_tagName;

	virtual void AddToParentObject (TiXmlElement* elem, const std::shared_ptr<Component>& component);
};

}	// namespace Engine

#endif	// #ifndef TAG_PROCESSOR_H
