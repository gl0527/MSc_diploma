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
	explicit		WeaponComponent (const std::string& name);

	void			PreUpdate (float t, float dt) override;

	unsigned char	GetAmmo () const;
	void			DecreaseAmmoByOne ();
	void			SetAmmoToFull ();
	bool			HasAmmo () const;

private:
	const unsigned char m_FullAmmo;
	unsigned char		m_ammo;
};

#endif	// #ifndef WEAPON_COMPONENT_H
