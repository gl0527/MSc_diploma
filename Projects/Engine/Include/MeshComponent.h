#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"


namespace Engine
{

// ============================= class MeshComponent =============================

	class DLL_SPEC MeshComponent : public RenderComponent
	{
	public:
		MeshComponent(const std::string& eName, const std::string& mName);
		virtual ~MeshComponent();

		virtual void Init(GameObject* obj) override;
		virtual void Destroy() override;
		
		void setMaterial(const std::string& matName);
		Ogre::Entity* getEntity() const { return entity; }

	private:
		static unsigned int instanceCount;
		Ogre::Entity* entity;
		std::string mesh;
	};

}	// namespace Engine

#endif	// #ifndef MESH_COMPONENT_H
