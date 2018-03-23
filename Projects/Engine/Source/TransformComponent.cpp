#include "TransformComponent.h"
#include "GameObject.h"


namespace Engine {

TransformComponent::TransformComponent (const std::string& name) :
	Component (name, true),
	m_globalPosition (Ogre::Vector3::ZERO),
	m_globalRotation (Ogre::Quaternion::IDENTITY),
	m_globalScale (Ogre::Vector3::UNIT_SCALE),
	m_globalTransform (Ogre::Matrix4::IDENTITY),
	m_localPosition (Ogre::Vector3::ZERO),
	m_localRotation (Ogre::Quaternion::IDENTITY),
	m_localScale (Ogre::Vector3::UNIT_SCALE),
	m_localTransform (Ogre::Matrix4::IDENTITY),
	m_pParentTransform (nullptr)
{
}


void TransformComponent::PostInit (GameObject* /*owner*/)
{
	if (auto parent = m_owner->GetParent ().lock ()) {
		m_pParentTransform = parent->Transform ();
	}
}


inline const Ogre::Vector3& TransformComponent::GetGlobalPosition () const
{
	return m_globalPosition;
}


inline const Ogre::Quaternion& TransformComponent::GetGlobalRotation () const
{
	return m_globalRotation;
}


inline const Ogre::Vector3& TransformComponent::GetGlobalScale () const
{
	return m_globalScale;
}


inline const Ogre::Vector3& TransformComponent::GetLocalPosition () const
{
	return m_localPosition;
}


inline const Ogre::Quaternion& TransformComponent::GetLocalRotation () const
{
	return m_localRotation;
}


inline const Ogre::Vector3& TransformComponent::GetLocalScale () const
{
	return m_localScale;
}


Ogre::Vector3 TransformComponent::Forward () const
{
	return m_globalRotation * Ogre::Vector3::NEGATIVE_UNIT_Z;
}


Ogre::Vector3 TransformComponent::Right () const
{
	return m_globalRotation * Ogre::Vector3::UNIT_X;
}


Ogre::Vector3 TransformComponent::Up () const
{
	return m_globalRotation * Ogre::Vector3::UNIT_Y;
}


void TransformComponent::SetGlobalPosition (const Ogre::Vector3& globalPosition)
{
	m_globalPosition = globalPosition;
	m_globalTransform.makeTransform (m_globalPosition, m_globalScale, m_globalRotation);
	UpdateLocalTransform ();
}


void TransformComponent::SetGlobalRotation (const Ogre::Quaternion& globalRotation)
{
	m_globalRotation = globalRotation;
	m_globalTransform.makeTransform (m_globalPosition, m_globalScale, m_globalRotation);
	UpdateLocalTransform ();
}


void TransformComponent::SetGlobalScale (const Ogre::Vector3& globalScale)
{
	m_globalScale = globalScale;
	m_globalTransform.makeTransform (m_globalPosition, m_globalScale, m_globalRotation);
	UpdateLocalTransform ();
}


void TransformComponent::SetLocalPosition (const Ogre::Vector3& localPosition)
{
	m_localPosition = localPosition;
	m_localTransform.makeTransform (m_localPosition, m_localScale, m_localRotation);
	UpdateGlobalTransform ();
}


void TransformComponent::SetLocalRotation (const Ogre::Quaternion& localRotation)
{
	m_localRotation = localRotation;
	m_localTransform.makeTransform (m_localPosition, m_localScale, m_localRotation);
	UpdateGlobalTransform ();
}


void TransformComponent::SetLocalScale (const Ogre::Vector3& localScale)
{
	m_localScale = localScale;
	m_localTransform.makeTransform (m_localPosition, m_localScale, m_localRotation);
	UpdateGlobalTransform ();
}


void TransformComponent::Translate (const Ogre::Vector3& translateVec)
{
	Ogre::Vector3 newVec = m_localPosition + translateVec;
	SetLocalPosition (newVec);
}


void TransformComponent::Rotate (const Ogre::Quaternion& rotQ)
{
	Ogre::Quaternion newRot = m_localRotation * rotQ;
	SetLocalRotation (newRot);
}


void TransformComponent::Rotate (float angleInRad, const Ogre::Vector3& axis)
{
	Ogre::Quaternion rot (Ogre::Radian (angleInRad), axis);
	Ogre::Quaternion newRot = m_localRotation * rot;
	SetLocalRotation (newRot);
}


void TransformComponent::Scale (const Ogre::Vector3& scaleVec)
{
	Ogre::Vector3 newScale (m_localScale.x * scaleVec.x, m_localScale.y * scaleVec.y, m_localScale.z * scaleVec.z);
	SetLocalScale (newScale);
}


void TransformComponent::UpdateGlobalTransform ()
{
	if (m_pParentTransform != nullptr) {
		m_globalTransform = m_pParentTransform->m_globalTransform * m_localTransform;
	} else {
		m_globalTransform = m_localTransform;
	}
	m_globalTransform.decomposition (m_globalPosition, m_globalScale, m_globalRotation);
	UpdateGlobalTransformForChildren ();
}


void TransformComponent::UpdateLocalTransform ()
{
	if (m_pParentTransform != nullptr) {
		m_localTransform = m_pParentTransform->m_globalTransform.inverse () * m_globalTransform;
	} else {
		m_localTransform = m_globalTransform;
	}
	m_localTransform.decomposition (m_localPosition, m_localScale, m_localRotation);
	UpdateGlobalTransformForChildren ();
}


void TransformComponent::UpdateGlobalTransformForChildren ()
{
	const auto& children = m_owner->GetChildren ();

	for (const auto& weakChild : children) {
		if (auto child = weakChild.lock ()) {
			child->Transform ()->UpdateGlobalTransform ();
		}
	}
}

}	// namespace Engine
