#ifndef BILLBOARD_COMPONENT_H
#define BILLBOARD_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "RenderComponent.h"


// ----------------------------- forward declaration -----------------------------

namespace Ogre {
class BillboardSet;
}

namespace Engine {

// ========================== class BillboardComponent ==========================

class BillboardComponent : public RenderComponent
{
public:
	BillboardComponent (const std::string& bbName);
	virtual ~BillboardComponent ();

	virtual void PostInit (GameObject* obj) override;
	virtual void Destroy () override;

	Ogre::BillboardSet* getBillboardSet () const { return billboardSet; }

private:
	static unsigned int instanceCount;
	Ogre::BillboardSet* billboardSet;
};

}	// namespace Engine

#endif	// #ifndef BILLBOARD_COMPONENT_H
