#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include "Ogre.h"


namespace Engine
{

// ========================== class TransformComponent ==========================

class TransformComponent : public Component
{
public:
	TransformComponent (const std::string& name) :
		Component (name),
		globalPos (Ogre::Vector3::ZERO),
		globalRot (Ogre::Quaternion::IDENTITY),
		globalScale (Ogre::Vector3::UNIT_SCALE)
	{}

	virtual void Start () override;

	// getter functions
	const Ogre::Vector3& worldPosition () const { return globalPos; }
	const Ogre::Quaternion& worldRotation () const { return globalRot; }
	const Ogre::Vector3& worldScale () const { return globalScale; }

	Ogre::Vector3 forward () const { return globalRot * Ogre::Vector3::NEGATIVE_UNIT_Z; }
	Ogre::Vector3 up () const { return globalRot * Ogre::Vector3::UNIT_Y; }
	Ogre::Vector3 right () const { return globalRot * Ogre::Vector3::UNIT_X; }

	// setter functions
	void setPosition (const Ogre::Vector3& p) { globalPos = p; }
	void setRotation (const Ogre::Quaternion& q) { globalRot = q; }
	void setScale (const Ogre::Vector3& s) { globalScale = s; }

	void translate (const Ogre::Vector3& p) { globalPos += p; }
	void rotate (const Ogre::Quaternion& q) { globalRot = globalRot * q; }
	void scaling (const Ogre::Vector3& s) { globalScale *= s; }

private:
	Ogre::Vector3 globalPos;
	Ogre::Quaternion globalRot;
	Ogre::Vector3 globalScale;
	Ogre::Matrix4 globalTrans;

	Ogre::Vector3 localPos;
	Ogre::Quaternion localRot;
	Ogre::Vector3 localScale;
	Ogre::Matrix4 localTrans;
};

}	// namespace Engine

#endif	// #ifndef TRANSFORM_COMPONENT_H
