#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"


using namespace Engine;

namespace Engine {

class AudioSourceComponent;

}	// namespace Engine

// =========================== class WeaponComponent ============================

class WeaponComponent : public Component
{
public:
	explicit		WeaponComponent (const std::string& name, unsigned char damage, unsigned char fullAmmo);

	void			Start () override;
	void			PreUpdate (float t, float dt) override;

	unsigned char	GetAmmo () const;
	void			SetAmmoToFull ();
	bool			HasAmmo () const;

	void			Shoot ();

private:
	const unsigned char						m_damage;
	const unsigned char						m_FullAmmo;
	unsigned char							m_ammo;
	std::shared_ptr<AudioSourceComponent>	m_pOwnerAudio;
};

#endif	// #ifndef WEAPON_COMPONENT_H
