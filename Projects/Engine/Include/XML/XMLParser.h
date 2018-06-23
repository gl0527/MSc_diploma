#pragma once

#ifndef XML_PARSER_H
#define XML_PARSER_H

// ---------------------------------- includes ----------------------------------

#include "tinyxml.h"
#include "stdafx.h"
#include "SingletonBase.h"
#include <map>
#include <string>
#include <sstream>


// ----------------------------- forward declaration -----------------------------

namespace Ogre {
class ColourValue;
class Quaternion;
class Vector3;
}	// namespace Ogre


namespace Engine {
namespace XML {

// ----------------------------- forward declaration -----------------------------

class TagProcessor;

// =============================== class XMLParser ===============================

class XMLParser final : public SingletonBase<XMLParser>
{
public:
	static	DLL_EXPORT	XMLParser&	GetInstance ();

	static	DLL_EXPORT	void		ParsePrimitive (TiXmlElement* tag, const char* attrName, float* outResult, bool optional = false);
	static	DLL_EXPORT	void		ParsePrimitive (TiXmlElement* tag, const char* attrName, int* outResult, bool optional = false);
	static	DLL_EXPORT	void		ParsePrimitive (TiXmlElement* tag, const char* attrName, const char** outResult, bool optional = false);
	static	DLL_EXPORT	void		ParsePrimitive (TiXmlElement* tag, const char* attrName, bool* outResult, bool optional = false);

	static	DLL_EXPORT	void		ParseFloat3_XYZ (TiXmlElement* tag, Ogre::Vector3& outResult);
	static	DLL_EXPORT	void		ParseFloat3_RGB (TiXmlElement* tag, Ogre::ColourValue& outResult);
	static	DLL_EXPORT	void		ParseFloat3_RGBA (TiXmlElement* tag, Ogre::ColourValue& outResult);
	static	DLL_EXPORT	void		ParseFloat4_WXYZ (TiXmlElement* tag, Ogre::Quaternion& outResult);

			DLL_EXPORT	void		AddXMLProcessor (TagProcessor* proc);

						bool		Init ();
			DLL_EXPORT	bool		LoadXMLFromFile (const char* fileName);
						void		Destroy ();

private:
	friend class SingletonBase<XMLParser>;

	using XMLProcessorMap = std::map<std::string, TagProcessor*>;

	XMLProcessorMap		m_xmlProcessorMap;
	TiXmlDocument		m_xmlDocument;
	TiXmlElement*		m_pDocumentRoot;


									XMLParser ();

						void		TraverseXMLTree (TiXmlElement* elem);
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef XML_PARSER_H
