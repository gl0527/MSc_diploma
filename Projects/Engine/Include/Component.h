#ifndef COMPONENT_H
#define COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include <string>
#include <memory>
#include "stdafx.h"


namespace Engine
{

// ----------------------------- forward declaration -----------------------------

class GameObject;

// =============================== class Component ===============================

class DLL_SPEC Component
{
public:
	using SPtr = std::shared_ptr<Component>;
	using WPtr = std::weak_ptr<Component>;
	using UPtr = std::unique_ptr<Component>;

	struct ConstructionData;

	// static SPtr Create (Prefab);

	Component (const std::string& ident, bool uniq = false);
	virtual ~Component ();

	// runs when this component is added to a gameobject
	virtual void	Init (GameObject* object);
	// runs just before the updating loop - every gameobject is ready
	virtual void	Start ();
	// runs on every update cycle - before the physics update
	virtual void	PreUpdate (float t, float dt);
	// runs on every update cycle - just before the physics update
	virtual void	Update (float t, float dt);
	// runs on every update cycle - after the physics update
	virtual void	PostUpdate (float t, float dt);
	virtual void	Destroy ();

	const std::string&	GetName () const;
	const bool			IsUnique () const;
	bool				IsEnabled () const;
	void				Enable ();
	void				Disable ();
	GameObject*			GetOwner () const;

protected:
	// friend class XYProcessor;

	const std::string	m_name;
	const bool			m_isUnique;
	volatile bool		m_isEnabled;
	GameObject*			m_owner;
	//std::shared_ptr<ConstructionData>	m_pConstructionData;

	// ezt hasznalja a memberek helyett, s ezt kell inicializalni a prefabbal
	// emellett nem is kellenenek a memberek
	struct MemberList
	{
		const std::string	m_name;
		const bool			m_isUnique;
		volatile bool		m_isEnabled;
		GameObject*			m_owner;
	};

	std::shared_ptr<MemberList> m_memberList;
};


struct Component::ConstructionData
{
	const std::string	m_name;
	//const bool			m_isUnique;
	//volatile bool		m_isEnabled;
	//GameObject*			m_owner;

	//GameObject* Component::*pOwner = &Component::m_owner;
};

/*bool Component::*pBool = &Component::m_isEnabled;*/

}	// namespace Engine

#endif	// #ifndef COMPONENT_H
