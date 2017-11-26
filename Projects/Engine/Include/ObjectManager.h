#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include <map>
#include <memory>
#include "stdafx.h"


namespace Engine {

// ----------------------------- forward declaration-----------------------------

class GameObject;

// ============================= class ObjectManager =============================

class ObjectManager
{
public:
	~ObjectManager ();

	static DLL_EXPORT ObjectManager&	GetSingletonInstance ();
	static void				DeleteSingletonInstance ();
	static bool				IsExist ();

	DLL_EXPORT std::weak_ptr<GameObject>	CreateGameObject (const std::string& id);
	DLL_EXPORT void						RemoveGameObject (const std::string& id);

	void	Start ();
	void	PreUpdate (float t, float dt);
	void	Update (float t, float dt);
	void	PostUpdate (float t, float dt);
	void	Destroy ();

	DLL_EXPORT std::weak_ptr<GameObject> GetGameObjectByName (const std::string& objName) const;

private:
	using GameObjectMap = std::map<std::string, std::shared_ptr<GameObject>>;

	static ObjectManager*	s_pInstance;

	GameObjectMap			m_gameObjectMap;
};

}	// namespace Engine

#endif	// #ifndef OBJECT_MANAGER_H
