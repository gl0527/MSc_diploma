#include "TransformComponent.h"
#include "GameObject.h"


namespace Engine {

TransformComponent::TransformComponent (const std::string& name) :
	Component (name, true),
	m_posInWorldSpace (Ogre::Vector3::ZERO),
	m_rotInWorldSpace (Ogre::Quaternion::IDENTITY),
	m_scaleInWorldSpace (Ogre::Vector3::UNIT_SCALE),
	m_hasParent (false)
{
}


void TransformComponent::Start ()
{
	if (auto parent = m_owner->GetParent ().lock ()) {
		m_hasParent = true;
		
		auto parentTrans = parent->Transform ();
		auto parentPos = parentTrans->GetPositionInWorldSpace ();

		m_posInParentSpace = m_posInWorldSpace - parentPos;
		m_rotInParentSpace = parentTrans->GetRotationInWorldSpace ().Inverse () * m_rotInWorldSpace;
		m_scaleInParentSpace = m_scaleInWorldSpace / parentTrans->GetScaleInWorldSpace ();
	} else {
		m_posInParentSpace = m_posInWorldSpace;
		m_rotInParentSpace = m_rotInWorldSpace;
		m_scaleInParentSpace = m_scaleInWorldSpace;
	}
}


const Ogre::Vector3& TransformComponent::GetPositionInWorldSpace () const
{
	return m_posInWorldSpace;
}


const Ogre::Quaternion& TransformComponent::GetRotationInWorldSpace () const
{
	return m_rotInWorldSpace;
}


const Ogre::Vector3& TransformComponent::GetScaleInWorldSpace () const
{
	return m_scaleInWorldSpace;
}


Ogre::Vector3 TransformComponent::GetPositionInCameraSpace (const Ogre::Matrix4& camViewMat) const
{
	return camViewMat * m_posInWorldSpace;
}


Ogre::Vector3 TransformComponent::GetForwardVecInWorldSpace () const
{
	return m_rotInWorldSpace * Ogre::Vector3::NEGATIVE_UNIT_Z;
}


Ogre::Vector3 TransformComponent::GetUpVecInWorldSpace () const
{
	return m_rotInWorldSpace * Ogre::Vector3::UNIT_Y;
}


Ogre::Vector3 TransformComponent::GetRightVecInWorldSpace () const
{
	return m_rotInWorldSpace * Ogre::Vector3::UNIT_X;
}


void TransformComponent::SetWorldPosition (const Ogre::Vector3& p)
{
	m_posInWorldSpace = p;
}


void TransformComponent::SetWorldRotation (const Ogre::Quaternion& q)
{
	m_rotInWorldSpace = q;
}


void TransformComponent::SetWorldScale (const Ogre::Vector3& s)
{
	m_scaleInWorldSpace = s;
}


void TransformComponent::SetPosInParentSpace (const Ogre::Vector3& pos)
{
	m_posInParentSpace = pos;
	if (!m_hasParent)
		m_posInWorldSpace = m_posInParentSpace;
}


void TransformComponent::SetRotInParentSpace (const Ogre::Quaternion& rot)
{
	m_rotInParentSpace = rot;
	if (!m_hasParent)
		m_rotInWorldSpace = m_rotInParentSpace;
}


void TransformComponent::SetScaleInParentSpace (const Ogre::Vector3& scale)
{
	m_scaleInParentSpace = scale;
	if (!m_hasParent)
		m_scaleInWorldSpace = m_scaleInParentSpace;
}


void TransformComponent::AddToWorldPosition (const Ogre::Vector3& p)
{
	m_posInWorldSpace += p;
}


void TransformComponent::AddToWorldRotation (const Ogre::Quaternion& q)
{
	m_rotInWorldSpace = m_rotInWorldSpace * q;
}


void TransformComponent::AddToWorldScale (const Ogre::Vector3& s)
{
	m_scaleInWorldSpace *= s;
}

}	// namespace Engine
