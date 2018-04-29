#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"


using namespace Engine;

// =========================== class WeaponComponent ============================

class WeaponComponent : public Component
{
public:
			WeaponComponent (const std::string& name);

	void	PreUpdate (float t, float dt) override;
};

#endif	// #ifndef WEAPON_COMPONENT_H
