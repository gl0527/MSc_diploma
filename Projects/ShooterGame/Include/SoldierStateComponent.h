#ifndef SOLDIER_STATE_COMPONENT_H
#define SOLDIER_STATE_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include "FiniteStateMachine.h"

using namespace Engine;

class GameObject;

// ========================= class SoldierStateComponent =========================

class SoldierStateComponent : public Component
{
public:
	enum class States { Follow, Move_Away };

	SoldierStateComponent (const std::string& name);

	void Start () override;
	void PreUpdate (float t, float dt) override;

	States GetState ();	// a tobbi komponens ez alapjan valtoztathatja a viselkedeset

private:
	using SoldierStateMachine = FiniteStateMachine<States, char>;
	SoldierStateMachine soldierStateMachine;
	std::shared_ptr<Engine::GameObject> m_leaderObj;
};

#endif	// #ifndef SOLDIER_ANIM_COMPONENT_H
