#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include "Ogre.h"


namespace Engine {

// ========================== class TransformComponent ==========================

class TransformComponent : public Component
{
public:
										TransformComponent (const std::string& name);

	void								PostInit (GameObject* /*owner*/) override;

	DLL_EXPORT Ogre::Vector3			Forward () const;
	DLL_EXPORT Ogre::Vector3			Right () const;
	DLL_EXPORT Ogre::Vector3			Up () const;
	
	DLL_EXPORT const Ogre::Vector3&		GetGlobalPosition () const;
	DLL_EXPORT const Ogre::Quaternion&	GetGlobalRotation () const;
	DLL_EXPORT const Ogre::Vector3&		GetGlobalScale () const;

	DLL_EXPORT const Ogre::Vector3&		GetLocalPosition () const;
	DLL_EXPORT const Ogre::Quaternion&	GetLocalRotation () const;
	DLL_EXPORT const Ogre::Vector3&		GetLocalScale () const;

	DLL_EXPORT void						SetGlobalPosition (const Ogre::Vector3& globalPosition);
	DLL_EXPORT void						SetGlobalRotation (const Ogre::Quaternion& globalRotation);
	DLL_EXPORT void						SetGlobalScale (const Ogre::Vector3& globalScale);

	DLL_EXPORT void						SetLocalPosition (const Ogre::Vector3& localPosition);
	DLL_EXPORT void						SetLocalRotation (const Ogre::Quaternion& localRotation);
	DLL_EXPORT void						SetLocalScale (const Ogre::Vector3& localScale);

	void								Translate (const Ogre::Vector3& translateVec);
	void								Rotate (const Ogre::Quaternion& rotQ);
	void								Rotate (float angleInRad, const Ogre::Vector3& axis);
	void								Scale (const Ogre::Vector3& scaleVec);

private:
	// values in world space
	Ogre::Vector3		m_globalPosition;
	Ogre::Quaternion	m_globalRotation;
	Ogre::Vector3		m_globalScale;
	Ogre::Matrix4		m_globalTransform;

	// values relative to the parent game object
	Ogre::Vector3		m_localPosition;
	Ogre::Quaternion	m_localRotation;
	Ogre::Vector3		m_localScale;
	Ogre::Matrix4		m_localTransform;

	TransformComponent* m_pParentTransform;

	void								UpdateGlobalTransform ();
	void								UpdateLocalTransform ();
};

}	// namespace Engine

#endif	// #ifndef TRANSFORM_COMPONENT_H
