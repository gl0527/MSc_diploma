#ifndef PREFAB_PROCESSOR_H
#define PREFAB_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"


namespace Engine {
namespace XML {

// ========================== class GameObjectProcessor ==========================

class PrefabProcessor : public TagProcessor
{
public:
					PrefabProcessor () : TagProcessor ("prefab") {}

	virtual bool	ProcessXMLTag (TiXmlElement* elem) override;
	virtual void	AddToParentObject (TiXmlElement* /*elem*/, const std::shared_ptr<Component>& /*component*/) {}
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef PREFAB_PROCESSOR_H