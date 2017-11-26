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
class PrefabBase;
class TransformComponent;

// =============================== class GameObject ===============================

class GameObject
{
public:
	using SPtr = std::shared_ptr<GameObject>;
	using WPtr = std::weak_ptr<GameObject>;
	using UPtr = std::unique_ptr<GameObject>;

	explicit GameObject (const std::string& id);
	~GameObject ();

	DLL_EXPORT void AddComponent (const std::shared_ptr<Component>& comp);
	DLL_EXPORT void removeComponent (const std::string& compName);
	void removeComponent (const std::shared_ptr<Component>& comp);
	void removeComponent ();

	void AddComponentPrefab (std::shared_ptr<PrefabBase> pPrefab);

	void addTag (const std::string& tag);
	void removeTag (const std::string& tag);
	void removeTag ();

	void addChild (const std::string& childName);
	void removeChild (const std::string& childName);
	void removeChildren ();

	void Start ();
	void PreUpdate (float t, float dt);
	void Update (float t, float dt);
	void PostUpdate (float t, float dt);
	void Destroy ();

	DLL_EXPORT const std::string&			GetName () const;
	DLL_EXPORT TransformComponent*			Transform () const;
	std::weak_ptr<Component>	GetComponent (const std::string& cID) const;
	DLL_EXPORT WPtr						GetParent () const;
	std::vector<std::string>	getChildrenNames () const;

	template<typename T>
	std::weak_ptr<T>			getFirstComponentByType ();

	void	clearParent ();
	bool	hasParent () const;
	void	setParent (const std::string& parentName);
	bool	hasTag (const std::string& t);
	bool	isDestroyed () const;

private:
	const std::string						m_Name;
	bool									m_isDestroyed;
	SPtr									m_pParent;
	std::vector<WPtr>						m_children;
	std::vector<std::shared_ptr<Component>>	m_components;
	std::unordered_set<std::string>			m_tags;

	std::vector<std::shared_ptr<PrefabBase>> m_pPrefabs;
};


template<typename T>
std::weak_ptr<T> GameObject::getFirstComponentByType ()
{
	for (auto it = m_components.begin (); it != m_components.end (); ++it) {
		if (auto casted = std::dynamic_pointer_cast<T>(*it))
			return casted;
	}
	return std::shared_ptr<T> (nullptr);
}

}	// namespace Engine

#endif	// #ifndef GAMEOBJECT_H
