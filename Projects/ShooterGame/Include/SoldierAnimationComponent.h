#pragma once

#ifndef SOLDIER_ANIMATION_COMPONENT_H
#define	SOLDIER_ANIMATION_COMPONENT_H

// ---------------------------------- includes ----------------------------------

#include "AnimationComponent.h"
#include "FiniteStateMachine.h"


using namespace Engine;

// ----------------------------- forward declaration -----------------------------

namespace Engine {

class GameObject;
class AudioSourceComponent;

}

class PlayerDataComponent;
class WeaponComponent;

// ====================== class SoldierAnimationComponent =======================

class SoldierAnimationComponent : public AnimationComponent
{
public:
	enum class UpperBodyState { Idle, Run, WeaponHold, Shoot, Dead };
	enum class LowerBodyState { Idle, Run, Dead };

	explicit	SoldierAnimationComponent (const std::string& name);

	void		Start () override;
	void		PostUpdate (float t, float dt) override;

private:
	using UpperBodyAnimation = FiniteStateMachine<UpperBodyState, char>;
	using LowerBodyAnimation = FiniteStateMachine<LowerBodyState, char>;

	UpperBodyAnimation						m_upperBodyAnimation;
	LowerBodyAnimation						m_lowerBodyAnimation;
	std::shared_ptr<PlayerDataComponent>	m_ownerData;
	std::shared_ptr<GameObject>				m_weapon;
	std::shared_ptr<WeaponComponent>		m_weaponComp;
	std::shared_ptr<AudioSourceComponent>	m_pOwnerAudio;
	bool									m_isInShootState;

	void	OnUpperBodyIdle (float t, float dt);
	void	OnUpperBodyRun (float t, float dt);
	void	OnUpperBodyShoot (float t, float dt);
	void	OnUpperBodyWeaponHold (float t, float dt);
	void	OnLowerBodyIdle (float t, float dt);
	void	OnLowerBodyRun (float t, float dt);
	void	OnDeath (float t, float dt);

	void	TransitionToDeath ();
};

#endif	//#ifndef SOLDIER_ANIMATION_COMPONENT_H
