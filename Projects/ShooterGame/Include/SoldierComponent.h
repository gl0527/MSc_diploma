#pragma once
#include "AnimationComponent.h"
#include "InputHandler.h"

using namespace Engine;

class SoldierComponent : public Component
{
public:
	enum class PlayerAction : unsigned char
	{
		PA_NONE,
		PA_WEAPON_HOLD,
		PA_SHOOT,
		PA_RUN
	};
private:
	PlayerAction action, lastAction;
	std::weak_ptr<AnimationComponent> animComp;
public:
	SoldierComponent(const std::string& name);
	~SoldierComponent();

	virtual void Start() override;
	virtual void PostUpdate(float t, float dt) override;

	void setAction(PlayerAction a) { action = a; }
};