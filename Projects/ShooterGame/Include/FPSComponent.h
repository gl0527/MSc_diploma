#pragma once
#include "Component.h"
#include "RenderSystem.h"

using namespace Engine;

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

