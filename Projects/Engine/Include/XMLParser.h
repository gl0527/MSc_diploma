#ifndef XML_PARSER_H
#define XML_PARSER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "tinyxml.h"
#include "stdafx.h"
#include <map>
#include <string>
#include <sstream>


// ----------------------------- forward declaration -----------------------------

namespace Ogre
{
	class ColourValue;
	class Quaternion;
	class Vector3;
}	// namespace Ogre


namespace Engine
{

// ----------------------------- forward declaration -----------------------------

class TagProcessor;

// =============================== class XMLParser ===============================

class DLL_EXPORT XMLParser
{
public:
	static XMLParser&	GetInstance ();
	static void			DeleteInstance ();
	static bool			IsExist ();

	static void	ParsePrimitive (TiXmlElement* tag, const char* attrName, float* outResult);
	static void	ParsePrimitive (TiXmlElement* tag, const char* attrName, int* outResult);
	static void	ParsePrimitive (TiXmlElement* tag, const char* attrName, const char** outResult);
	static void	ParsePrimitive (TiXmlElement* tag, const char* attrName, bool* outResult);

	static void	ParseFloat3_XYZ (TiXmlElement* tag, Ogre::Vector3& outResult);
	static void ParseFloat3_RGB (TiXmlElement* tag, Ogre::ColourValue& outResult);
	static void ParseFloat3_RGBA (TiXmlElement* tag, Ogre::ColourValue& outResult);
	static void ParseFloat4_WXYZ (TiXmlElement* tag, Ogre::Quaternion& outResult);

	void AddXMLProcessor (TagProcessor* proc);

	bool Init ();
	bool LoadXMLFromFile (const char* fileName);
	void Destroy ();

private:
	using XMLProcessorMap = std::map<std::string, TagProcessor*>;

	static XMLParser*	s_pInstance;

	XMLProcessorMap		m_xmlProcessorMap;
	TiXmlDocument		m_xmlDocument;
	TiXmlElement*		m_pDocumentRoot;


	XMLParser ();

	void TraverseXMLTree (TiXmlElement* elem);
};

}	// namespace Engine

#endif	// #ifndef XML_PARSER_H
