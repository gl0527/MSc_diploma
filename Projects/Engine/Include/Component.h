#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

// ---------------------------------- includes ----------------------------------

#include <string>
#include <memory>
#include "stdafx.h"


namespace Engine {

// ----------------------------- forward declaration -----------------------------

class GameObject;

// =============================== class Component ===============================

class Component
{
public:
	using SPtr = std::shared_ptr<Component>;
	using WPtr = std::weak_ptr<Component>;
	using UPtr = std::unique_ptr<Component>;

	DLL_EXPORT						Component (const std::string& ident, bool uniq = false);
	virtual DLL_EXPORT				~Component ();

	// runs when this component is added to a gameobject
	void							Init (GameObject* object);
	virtual DLL_EXPORT void			PostInit (GameObject* object);
	// runs just before the updating loop - every gameobject is ready
	virtual DLL_EXPORT void			Start ();
	// runs on every update cycle - before the physics update
	virtual DLL_EXPORT void			PreUpdate (float t, float dt);
	// runs on every update cycle - just before the physics update
	virtual DLL_EXPORT void			Update (float t, float dt);
	// runs on every update cycle - after the physics update
	virtual DLL_EXPORT void			PostUpdate (float t, float dt);
	virtual DLL_EXPORT void			Destroy ();

	DLL_EXPORT const std::string&	GetName ()		const;
	DLL_EXPORT const bool			IsUnique ()		const;
	bool							IsEnabled ()	const;
	void							Enable ();
	void							Disable ();
	bool							IsRemovable ()	const;
	DLL_EXPORT GameObject*			GetOwner ()		const;

	DLL_EXPORT std::string			GetTypeName ()	const;

	void							MarkForRemove ();

protected:
	const std::string	m_name;
	const bool			m_isUnique;
	volatile bool		m_isEnabled;
	bool				m_isRemovable;
	GameObject*			m_owner;
};

}	// namespace Engine

#endif	// #ifndef COMPONENT_H
