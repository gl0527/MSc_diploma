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

	virtual void						Start () override;

	DLL_EXPORT const Ogre::Vector3&		GetPositionInWorldSpace () const;
	DLL_EXPORT const Ogre::Quaternion&	GetRotationInWorldSpace () const;
	DLL_EXPORT const Ogre::Vector3&		GetScaleInWorldSpace () const;

	Ogre::Vector3						GetPositionInCameraSpace (const Ogre::Matrix4& camViewMat) const;	// TODO lehet, hogy a parameteratadas helyett lekerdezek kene a fuggveny torzseben
	Ogre::Quaternion					GetRotationInCameraSpace (const Ogre::Matrix4& camViewMat) const;
	Ogre::Vector3						GetScaleInCameraSpace (const Ogre::Matrix4& camViewMat) const;

	Ogre::Vector2						GetPositionInNDC (const Ogre::Matrix4& camViewProjMat) const;
	Ogre::Quaternion					GetRotationInNDC (const Ogre::Matrix4& camViewProjMat) const;
	Ogre::Vector2						GetScaleInNDC (const Ogre::Matrix4& camViewProjMat) const;

	DLL_EXPORT Ogre::Vector3			GetForwardVecInWorldSpace () const;
	Ogre::Vector3						GetUpVecInWorldSpace () const;
	DLL_EXPORT Ogre::Vector3			GetRightVecInWorldSpace () const;

	DLL_EXPORT void						SetWorldPosition (const Ogre::Vector3& p);
	DLL_EXPORT void						SetWorldRotation (const Ogre::Quaternion& q);
	DLL_EXPORT void						SetWorldScale (const Ogre::Vector3& s);

	// TODO XML-ben mindent parent space-ben adok meg, ami megegyezik a world space-szel akkor, ha nincs parent
	void								SetPosInParentSpace (const Ogre::Vector3& pos);
	void								SetRotInParentSpace (const Ogre::Quaternion& rot);
	void								SetScaleInParentSpace (const Ogre::Vector3& scale);

	void								AddToWorldPosition (const Ogre::Vector3& p);
	void								AddToWorldRotation (const Ogre::Quaternion& q);
	void								AddToWorldScale (const Ogre::Vector3& s);

private:
	Ogre::Vector3		m_posInWorldSpace;
	Ogre::Quaternion	m_rotInWorldSpace;
	Ogre::Vector3		m_scaleInWorldSpace;
	Ogre::Matrix4		m_modelTransform;

	Ogre::Vector3		m_posInParentSpace;
	Ogre::Quaternion	m_rotInParentSpace;
	Ogre::Vector3		m_scaleInParentSpace;
	Ogre::Matrix4		m_localTransform;

	bool				m_hasParent;
};

}	// namespace Engine

#endif	// #ifndef TRANSFORM_COMPONENT_H
