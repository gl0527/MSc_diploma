#ifndef BILLBOARD_COMPONENT_H
#define BILLBOARD_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"


namespace Engine
{

// ========================== class BillboardComponent ==========================

class DLL_SPEC BillboardComponent : public RenderComponent
{
public:
	BillboardComponent (const std::string& bbName);
	virtual ~BillboardComponent ();

	virtual void Init (GameObject* obj) override;
	virtual void Destroy () override;

	Ogre::BillboardSet* getBillboardSet () const { return billboardSet; }

private:
	static unsigned int instanceCount;
	Ogre::BillboardSet* billboardSet;
};

}	// namespace Engine

#endif	// #ifndef BILLBOARD_COMPONENT_H
