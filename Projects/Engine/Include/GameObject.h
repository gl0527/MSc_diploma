#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_set>


namespace Engine {

// ----------------------------- forward declaration -----------------------------

class Component;
class TransformComponent;

// =============================== class GameObject ===============================

class GameObject
{
public:
	using SPtr = std::shared_ptr<GameObject>;
	using WPtr = std::weak_ptr<GameObject>;
	using UPtr = std::unique_ptr<GameObject>;

	explicit						GameObject (const std::string& id);
									~GameObject ();

	DLL_EXPORT	void				AddComponent (const std::shared_ptr<Component>& comp, bool replaceOld = true);
				void				InsertComponent (size_t index, const std::shared_ptr<Component>& comp);
	DLL_EXPORT	void				RemoveComponent (const std::string& compName);
	void							RemoveComponent (const std::shared_ptr<Component>& comp);
	void							RemoveComponent ();

	void							AddTag (const std::string& tag);
	void							RemoveTag (const std::string& tag);
	void							RemoveTag ();

	void							AddChild (const std::string& childName);
	void							RemoveChild (const std::string& childName);
	void							RemoveChildren ();

	void							Start ();
	void							PreUpdate (float t, float dt);
	void							Update (float t, float dt);
	void							PostUpdate (float t, float dt);
	void							Destroy ();

	DLL_EXPORT	const std::string&	GetName () const;
	DLL_EXPORT	TransformComponent*	Transform () const;
	std::weak_ptr<Component>		GetComponent (const std::string& cID) const;
	DLL_EXPORT	WPtr				GetParent () const;
	std::vector<std::string>		GetChildrenNames () const;

	template<typename T>
	std::weak_ptr<T>				GetFirstComponentByType ();

	void							ClearParent ();
	bool							HasParent () const;
	void							SetParent (const std::string& parentName);
	bool							HasTag (const std::string& t) const;
	bool							IsDestroyed () const;

private:
	const std::string						m_Name;
	bool									m_isDestroyed;
	SPtr									m_pParent;
	std::vector<WPtr>						m_children;
	std::vector<std::shared_ptr<Component>>	m_components;
	std::unordered_set<std::string>			m_tags;
};


template<typename T>
std::weak_ptr<T> GameObject::GetFirstComponentByType ()
{
	for (auto it = m_components.begin (); it != m_components.end (); ++it) {
		if (auto casted = std::dynamic_pointer_cast<T>(*it))
			return casted;
	}

	return std::shared_ptr<T> (nullptr);
}

}	// namespace Engine

#endif	// #ifndef GAMEOBJECT_H
