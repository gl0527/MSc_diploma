#include "XML/XMLParser.h"

#include "Ogre.h"

#include "XML/TransformProcessor.h"
#include "XML/CameraProcessor.h"
#include "XML/MeshProcessor.h"
#include "XML/TPCameraProcessor.h"
#include "XML/PhysicsProcessor.h"
#include "XML/GameObjectProcessor.h"
#include "XML/AudioProcessor.h"
#include "XML/ParticleProcessor.h"
#include "XML/LightProcessor.h"
#include "XML/PrefabProcessor.h"


namespace Engine {
namespace XML {

XMLParser::XMLParser () : m_pDocumentRoot (nullptr)
{
}


XMLParser& XMLParser::GetInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (new XMLParser); });
	return *s_pInstance.get ();
}


void XMLParser::AddXMLProcessor (TagProcessor* proc)
{
	if (m_xmlProcessorMap.find (proc->GetTagName ()) == m_xmlProcessorMap.end ())
		m_xmlProcessorMap[proc->GetTagName ()] = proc;
}


bool XMLParser::Init ()
{
	new GameObjectProcessor;
	new TransformProcessor;
	new TPCameraProcessor;
	new CameraProcessor;
	new MeshProcessor;
	new PhysicsProcessor;
	new AudioProcessor;
	new ParticleProcessor;
	new LightProcessor;
	new PrefabProcessor;

	return true;
}


bool XMLParser::LoadXMLFromFile (const char* fileName)
{
	if (!m_xmlDocument.LoadFile (fileName))
		throw std::runtime_error ("The XML document does not exist or invalid.\n");

	m_pDocumentRoot = m_xmlDocument.FirstChildElement ();
	if (m_pDocumentRoot == nullptr) {
		throw std::runtime_error ("XML document root not found.\n");
	}
	TraverseXMLTree (m_pDocumentRoot);

	return true;
}


void XMLParser::Destroy ()
{
	for (auto it = m_xmlProcessorMap.begin (); it != m_xmlProcessorMap.end (); it++)
		delete it->second;
	m_xmlProcessorMap.clear ();
}


void XMLParser::TraverseXMLTree (TiXmlElement* elem)
{
	for (auto child = elem->FirstChildElement (); child != nullptr; child = child->NextSiblingElement ()) {
		std::string tagName (child->Value ());

		if (m_xmlProcessorMap[tagName])
			m_xmlProcessorMap[tagName]->ProcessXMLTag (child);

		TraverseXMLTree (child);
	}
}


void XMLParser::ParsePrimitive (TiXmlElement* tag, const char* attrName, float* outResult, bool optional/* = false*/)
{
	if (tag->QueryFloatAttribute (attrName, outResult) != 0) {
		std::string errorMsg ("[ *** Float parsing error *** ]\t\"" + std::string (attrName) +
			"\" attribute of \"" + std::string (tag->Value ()) + "\" tag not found.");

		if (!optional)
			throw std::runtime_error (errorMsg.c_str ());
		else
			std::cout << errorMsg << std::endl;
	}
}


void XMLParser::ParsePrimitive (TiXmlElement* tag, const char* attrName, int* outResult, bool optional/* = false*/)
{
	if (tag->QueryIntAttribute (attrName, outResult) != 0) {
		std::string errorMsg ("[ *** Int parsing error *** ]\t\"" + std::string (attrName) +
			"\" attribute of \"" + std::string (tag->Value ()) + "\" tag not found.");

		if (!optional)
			throw std::runtime_error (errorMsg.c_str ());
		else
			std::cout << errorMsg << std::endl;
	}
}


void XMLParser::ParsePrimitive (TiXmlElement* tag, const char* attrName, const char** outResult, bool optional/* = false*/)
{
	*outResult = tag->Attribute (attrName);
	if (*outResult == nullptr) {
		std::string errorMsg ("[ *** String parsing error *** ]\t\"" + std::string (attrName) +
			"\" attribute of \"" + std::string (tag->Value ()) + "\" tag not found.");

		if (!optional)
			throw std::runtime_error (errorMsg.c_str ());
		else
			std::cout << errorMsg << std::endl;
	}
}


void XMLParser::ParsePrimitive (TiXmlElement* tag, const char* attrName, bool* outResult, bool optional/* = false*/)
{
	if (tag->QueryBoolAttribute (attrName, outResult) != 0) {
		std::string errorMsg ("[ *** Boolean parsing error *** ]\t\"" + std::string (attrName) +
			"\" attribute of \"" + std::string (tag->Value ()) + "\" tag not found.");

		if (!optional)
			throw std::runtime_error (errorMsg.c_str ());
		else
			std::cout << errorMsg << std::endl;
	}
}


void XMLParser::ParseFloat3_XYZ (TiXmlElement* tag, Ogre::Vector3& outResult)
{
	float x, y, z;

	ParsePrimitive (tag, "x", &x);
	ParsePrimitive (tag, "y", &y);
	ParsePrimitive (tag, "z", &z);

	outResult = { x, y, z };
}


void XMLParser::ParseFloat3_RGB (TiXmlElement* tag, Ogre::ColourValue& outResult)
{
	float r, g, b;

	ParsePrimitive (tag, "r", &r);
	ParsePrimitive (tag, "g", &g);
	ParsePrimitive (tag, "b", &b);

	outResult.r = r;
	outResult.g = g;
	outResult.b = b;
}


void XMLParser::ParseFloat3_RGBA (TiXmlElement* tag, Ogre::ColourValue& outResult)
{
	float r, g, b, a;

	ParsePrimitive (tag, "r", &r);
	ParsePrimitive (tag, "g", &g);
	ParsePrimitive (tag, "b", &b);
	ParsePrimitive (tag, "a", &a);

	outResult.r = r;
	outResult.g = g;
	outResult.b = b;
	outResult.a = a;
}


void XMLParser::ParseFloat4_WXYZ (TiXmlElement* tag, Ogre::Quaternion& outResult)
{
	float qw, qx, qy, qz;

	ParsePrimitive (tag, "qw", &qw);
	ParsePrimitive (tag, "qx", &qx);
	ParsePrimitive (tag, "qy", &qy);
	ParsePrimitive (tag, "qz", &qz);

	outResult = { qw, qx, qy, qz };
}

}	// namespace XML
}	// namespace Engine
