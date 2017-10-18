#include "ParticleProcessor.h"
#include "ParticleComponent.h"
#include "XMLParser.h"


namespace Engine
{

bool ParticleProcessor::ProcessXMLTag (TiXmlElement* elem)
{
	const char* name = nullptr;
	const char* particle = nullptr;

	try {
		XMLParser::ParsePrimitive (elem, "name", &name);
		XMLParser::ParsePrimitive (elem, "particle", &particle);
	} catch (const std::runtime_error& re) {
		std::cout << re.what () << std::endl;

		return false;
	}
	std::shared_ptr<ParticleComponent> comp (new ParticleComponent (name, particle));

	AddToParentObject (elem, comp);

	return true;
}

}	// namespace Engine
