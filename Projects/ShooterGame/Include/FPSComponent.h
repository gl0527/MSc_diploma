#ifndef FPS_COMPONENT_H
#define FPS_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include "RenderSystem.h"

using namespace Engine;

namespace Ogre {
class TextAreaOverlayElement;
}

// ============================= class FPSComponent ==============================

class FPSComponent : public Component
{
	RenderSystem* renderSys;
	Ogre::RenderWindow* renderWnd;
	Ogre::TextAreaOverlayElement* text;
public:
	FPSComponent(const std::string& name);
	~FPSComponent();

	virtual void PostUpdate(float t, float dt) override;
};

#endif	// #ifndef FPS_COMPONENT_H
