#pragma once

#ifndef GAME_OBJECT_CREATOR_H
#define GAME_OBJECT_CREATOR_H

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include <vector>
#include <memory>


namespace Engine {
namespace Prefab {

class IPrefab;

// ========================== class GameObjectCreator ==========================

class GameObjectCreator
{
public:
	explicit			GameObjectCreator (const std::string& name);

	void				AddComponentCreator (std::shared_ptr<IPrefab> compCreator);
	DLL_EXPORT void		Instantiate (const std::string& name) const;
	
	const std::string&	GetName () const;

private:
	std::vector<std::shared_ptr<IPrefab>>	m_compCreatorVec;
	const std::string						m_name;
};

}	// namespace Prefab
}	// namespace Engine

#endif	//#ifndef GAME_OBJECT_CREATOR_H
