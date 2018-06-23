#pragma once

#ifndef PLAYER_DATA_COMPONENT_H
#define PLAYER_DATA_COMPONENT_H

// ---------------------------------- includes ----------------------------------

#include "Component.h"


using namespace Engine;

// ========================== class PlayerDataComponent =========================

class PlayerDataComponent : public Component
{
public:
					PlayerDataComponent (const std::string& name, unsigned char healtPoint);

	unsigned char	GetHealthPoint () const;
	unsigned char	GetKills () const;
	bool			IsDead () const;

	void			SetHealthPoint (unsigned char healthPoint);
	void			IncreaseHealtPoint (unsigned char amount);
	void			DecreaseHealtPoint (unsigned char amount);
	void			IncreaseKills (unsigned char amount);

	void			SetWeapon (bool hasWeapon);
	bool			HasWeapon () const;

private:
	unsigned char	m_healthPoint;
	unsigned char	m_kills;
	bool			m_hasWeapon;
};

#endif	// #ifndef PLAYER_DATA_COMPONENT_H
