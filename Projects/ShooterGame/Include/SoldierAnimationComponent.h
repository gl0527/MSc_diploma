#ifndef SOLDIER_ANIMATION_COMPONENT_H
#define	SOLDIER_ANIMATION_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "AnimationComponent.h"
#include "FiniteStateMachine.h"


using namespace Engine;

// ----------------------------- forward declaration -----------------------------

class PlayerDataComponent;
class ManagerComponent;

// ====================== class SoldierAnimationComponent =======================

class SoldierAnimationComponent : public AnimationComponent
{
public:
	enum class UpperBodyState { Idle, Run, WeaponHold, Shoot, Dead };
	enum class LowerBodyState { Idle, Run, Dead };

	explicit	SoldierAnimationComponent (const std::string& name);

	void		Start () override;
	void		PostUpdate (float t, float dt) override;

	void		HasWeapon (bool hasWeapon);
	bool		IsDead () const;

private:
	using UpperBodyAnimation = FiniteStateMachine<UpperBodyState, char>;
	using LowerBodyAnimation = FiniteStateMachine<LowerBodyState, char>;

	UpperBodyAnimation							m_upperBodyAnimation;
	LowerBodyAnimation							m_lowerBodyAnimation;
	std::shared_ptr<PlayerDataComponent>		m_ownerData;
	std::shared_ptr<ManagerComponent>			m_managerComp;
	bool										m_isInShootState;
	bool										m_hasWeapon;
	bool										m_isDead;

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
