#ifndef SOLDIER_ANIM_COMPONENT_H
#define SOLDIER_ANIM_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Stateable.h"
#include "AnimState.h"
#include "Component.h"
#include "Ogre.h"


using namespace Engine;

// ========================= class SoldierAnimComponent =========================

class SoldierAnimComponent : public Component, public Stateable
{
public:
	explicit SoldierAnimComponent (const std::string& name);
	~SoldierAnimComponent ();

	virtual void Start () override;
	virtual void PostUpdate (float t, float dt) override;

private:

// =============================== class IdleState ===============================

	class IdleState final : public AnimState
	{
	public:
		IdleState (SoldierAnimComponent* parent);

		void Execute (Stateable* stateable, float t, float dt) override;

	private:
		SoldierAnimComponent* m_pParent;
	};

// ================================ class RunState ================================

	class RunState final : public AnimState
	{
	public:
		RunState (SoldierAnimComponent* parent);

		void Enter (Stateable* stateable) override;
		void Execute (Stateable* stateable, float t, float dt) override;

	private:
		SoldierAnimComponent* m_pParent;
	};

// =============================== class ShootState ===============================

	class ShootState final : public AnimState
	{
	public:
		ShootState (SoldierAnimComponent* parent);

		void Enter (Stateable* stateable) override;
		void Execute (Stateable* stateable, float t, float dt) override;

	private:
		SoldierAnimComponent* m_pParent;
	};

	Ogre::Entity* m_entity;	// ennek engine oldalon kellene lennie
};

#endif	// #ifndef SOLDIER_ANIM_COMPONENT_H
