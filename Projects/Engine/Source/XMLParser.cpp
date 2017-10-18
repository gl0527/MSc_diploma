#include "XMLParser.h"

#include "Ogre.h"

#include "TransformProcessor.h"
#include "CameraProcessor.h"
#include "MeshProcessor.h"
#include "TPCameraProcessor.h"
#include "PhysicsProcessor.h"
#include "GameObjectProcessor.h"
#include "AudioProcessor.h"
#include "ParticleProcessor.h"
#include "LightProcessor.h"


namespace Engine
{

XMLParser* XMLParser::s_pInstance = nullptr;


XMLParser::XMLParser () : m_pDocumentRoot (nullptr)
{
}


XMLParser& XMLParser::GetInstance ()
{
	if (!s_pInstance)
		s_pInstance = new XMLParser;
	return *s_pInstance;
}


void XMLParser::DeleteInstance ()
{
	if (s_pInstance)
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}
}


bool XMLParser::IsExist ()
{
	return (s_pInstance != nullptr);
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

	return true;
}


bool XMLParser::LoadXMLFromFile (const char* fileName)
{
	if (!m_xmlDocument.LoadFile (fileName))
		throw std::runtime_error ("The XML document does not exist or invalid.\n");
	
	m_pDocumentRoot = m_xmlDocument.FirstChildElement ();
	if (m_pDocumentRoot == nullptr)
	{
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
	for (auto child = elem->FirstChildElement (); child != nullptr; child = child->NextSiblingElement ())
	{
		std::string tagName (child->Value ());

		if (m_xmlProcessorMap[tagName])
			m_xmlProcessorMap[tagName]->ProcessXMLTag (child);

		TraverseXMLTree (child);
	}
}


void XMLParser::ParsePrimitive (TiXmlElement* tag, const char* attrName, float* outResult)
{
	if (tag->QueryFloatAttribute (attrName, outResult) != 0)
	{
		std::string errorMsg ("Float parsing error: " + std::string (attrName) +
			" attribute of " + std::string (tag->Value ()) + " tag not found.");

		throw std::runtime_error (errorMsg.c_str ());
	}
}


void XMLParser::ParsePrimitive (TiXmlElement* tag, const char* attrName, int* outResult)
{
	if (tag->QueryIntAttribute (attrName, outResult) != 0)
	{
		std::string errorMsg ("Int parsing error: " + std::string (attrName) + 
			" attribute of " + std::string (tag->Value ()) + " tag not found.");
		
		throw std::runtime_error (errorMsg.c_str ());
	}
}


void XMLParser::ParsePrimitive (TiXmlElement* tag, const char* attrName, const char** outResult)
{
	*outResult = tag->Attribute (attrName);
	if (*outResult == nullptr)
	{
		std::string errorMsg ("String parsing error: " + std::string (attrName) +
			" attribute of " + std::string (tag->Value ()) + " tag not found.");
		
		throw std::runtime_error (errorMsg.c_str ());
	}
}


void XMLParser::ParsePrimitive (TiXmlElement* tag, const char* attrName, bool* outResult)
{
	if (tag->QueryBoolAttribute (attrName, outResult) != 0)
	{
		std::string errorMsg ("Boolean parsing error: " + std::string (attrName) +
			" attribute of " + std::string (tag->Value ()) + " tag not found.");
		
		throw std::runtime_error (errorMsg.c_str ());
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
	
	outResult = {qw, qx, qy, qz};
}

}	// namespace Engine
