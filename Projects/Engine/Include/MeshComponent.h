#pragma once

#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

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
	struct Descriptor
	{
		Descriptor ();
		
		std::string entityName;
		std::string meshFileName;

		std::string materialName;
	};

								MeshComponent (const std::string& eName, const std::string& mName);
								MeshComponent (const Descriptor& desc);
	virtual						~MeshComponent ();

	virtual void				PostInit (GameObject* obj) override;
	virtual	void				Destroy () override;

	void						ApplyDescriptor (const Descriptor& desc);

	void						SetMaterial (const std::string& matName);
	DLL_EXPORT Ogre::Entity*	GetEntity () const;

private:
	static unsigned int	s_instanceCount;
	
	Ogre::Entity*		m_pEntity;
	std::string			m_meshName;
};

}	// namespace Engine

#endif	// #ifndef MESH_COMPONENT_H
