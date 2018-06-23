#pragma once

#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

// ---------------------------------- includes ----------------------------------

#include <functional>
#include <vector>
#include <map>
#include "Delegate.h"


namespace Engine {

// ========================== class FiniteStateMachine ==========================

template<typename State, typename Input>
class FiniteStateMachine
{
public:
	using TransitionBehaviour	= std::function<void ()>;
	using StateBehaviour		= std::function<void (float, float)>;
	using StateDelegate			= Delegate<void, float, float>;

	struct Transition
	{
		State				sourceState;
		State				destinationState;
		Input				input;
		TransitionBehaviour actionFn;
	};

	explicit FiniteStateMachine (State initState) :
		m_transitions (),
		m_stateFunctions (),
		m_state (initState)
	{
	}

	State GetState () const
	{
		return m_state;
	}

	void SetState (State state)
	{
		m_state = state;
	}

	void AddTransition (Transition transition)
	{
		m_transitions[transition.sourceState].push_back (transition);
	}

	void AddTransitions (std::initializer_list<Transition>&& transitionList)
	{
		for (auto&& trans : transitionList)
			AddTransition (trans);
	}

	void AddStateFunction (State state, StateBehaviour stateBehaviour)
	{
		m_stateFunctions[state] += stateBehaviour;
	}

	bool Process (Input input)
	{
		const auto it = m_transitions.find (m_state);
		if (it == m_transitions.end ())
			return false;

		const TransitionVec& currentTransitions = it->second;
		for (const Transition& trans : currentTransitions) {
			if (trans.input != input)
				continue;
			if (trans.actionFn != nullptr)
				trans.actionFn ();
			m_state = trans.destinationState;
			break;
		}

		return true;
	}

	void Update (float t, float dt)
	{
		m_stateFunctions[m_state] (t, dt);
	}

private:
	using TransitionVec = std::vector<Transition>;
	using TransitionMap = std::map<State, TransitionVec>;
	using StateFnMap = std::map<State, StateDelegate>;

	TransitionMap	m_transitions;
	StateFnMap		m_stateFunctions;
	State			m_state;
};

}	// namespace Engine

#endif
