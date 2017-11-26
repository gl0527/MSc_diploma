#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"


// ----------------------------- forward declaration -----------------------------

namespace Ogre {
class Entity;
}

namespace Engine {

// ============================= class MeshComponent =============================

class MeshComponent : public RenderComponent
{
public:
	MeshComponent (const std::string& eName, const std::string& mName);
	virtual ~MeshComponent ();

	virtual void PostInit (GameObject* obj) override;
	virtual void Destroy () override;

	void setMaterial (const std::string& matName);
	Ogre::Entity* getEntity () const { return entity; }

private:
	static unsigned int instanceCount;
	Ogre::Entity* entity;
	std::string mesh;
};

}	// namespace Engine

#endif	// #ifndef MESH_COMPONENT_H
