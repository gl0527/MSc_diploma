#ifndef COMPONENT_CREATOR_H
#define COMPONENT_CREATOR_H

#pragma once

namespace Engine {

// ----------------------------- forward declaration -----------------------------

class GameObject;

// =========================== class ComponentCreator ===========================

class ComponentCreator
{
public:
	virtual void Create (GameObject* gameObject) = 0;
	virtual void ApplyInitData () = 0;
};

}	// namespace Engine

#endif	// #ifndef COMPONENT_CREATOR_H
