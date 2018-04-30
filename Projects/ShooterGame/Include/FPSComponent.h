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
public:
	explicit	FPSComponent(const std::string& name);

	void		PostUpdate(float t, float dt) override;

private:
	RenderSystem* renderSys;
	Ogre::RenderWindow* renderWnd;
	Ogre::TextAreaOverlayElement* text;
};

#endif	// #ifndef FPS_COMPONENT_H
