#ifndef GAME_OBJECT_PROCESSOR_H
#define GAME_OBJECT_PROCESSOR_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "XML/TagProcessor.h"


namespace Engine {
namespace XML {

// ========================== class GameObjectProcessor ==========================

class GameObjectProcessor : public TagProcessor
{
public:
					GameObjectProcessor () : TagProcessor ("gameobject") {}

	virtual bool	ProcessXMLTag (TiXmlElement* elem) override;
	virtual void	AddToParentObject (TiXmlElement* /*elem*/, const std::shared_ptr<Component>& /*component*/) {}
};

}	// namespace XML
}	// namespace Engine

#endif	// #ifndef GAME_OBJECT_PROCESSOR_H
