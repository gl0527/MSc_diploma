#pragma once

#ifndef SOLDIER_ANIM_COMPONENT_H
#define SOLDIER_ANIM_COMPONENT_H

// ---------------------------------- includes ----------------------------------

#include "Stateable.h"
#include "AnimState.h"
#include "Component.h"

// ----------------------------- forward declaration -----------------------------

namespace Ogre
{
class Entity;
}


using namespace Engine;

// ========================= class SoldierAnimComponent =========================

class SoldierAnimComponent : public Stateable
{
public:
	explicit SoldierAnimComponent (const std::string& name);
	~SoldierAnimComponent ();

	virtual void Start () override;

private:

// =============================== class IdleState ===============================

	class IdleState final : public AnimState
	{
	public:
		static const SPtr&	GetInstance (SoldierAnimComponent* parent);
		static void			DeleteInstance ();
		static bool			HasInstance ();
		
		void PostExecute (Stateable* stateable, float t, float dt) override;

	private:
		static SPtr s_pInstance;
		SoldierAnimComponent* m_pParent;

		IdleState (SoldierAnimComponent* parent);
	};

// ================================ class RunState ================================

	class RunState final : public AnimState
	{
	public:
		static const SPtr&	GetInstance (SoldierAnimComponent* parent);
		static void			DeleteInstance ();
		static bool			HasInstance ();

		void PostEnter (Stateable* stateable) override;
		void PostExecute (Stateable* stateable, float t, float dt) override;

	private:
		static SPtr s_pInstance;
		SoldierAnimComponent* m_pParent;

		RunState (SoldierAnimComponent* parent);
	};

// =============================== class ShootState ===============================

	class ShootState final : public AnimState
	{
	public:
		static const SPtr&	GetInstance (SoldierAnimComponent* parent);
		static void			DeleteInstance ();
		static bool			HasInstance ();

		void PostEnter (Stateable* stateable) override;
		void PostExecute (Stateable* stateable, float t, float dt) override;

	private:
		static SPtr s_pInstance;
		SoldierAnimComponent* m_pParent;

		ShootState (SoldierAnimComponent* parent);
	};


	Ogre::Entity* m_entity;	// ennek engine oldalon kellene lennie
};

#endif	// #ifndef SOLDIER_ANIM_COMPONENT_H
