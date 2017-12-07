#include "PhysicsMaterial.h"


namespace Engine {

PhysicsMaterial::PhysicsMaterial ()
	: m_angularDamping (0.0f),
	m_bounciness (0.0f),
	m_friction (0.0f),
	m_linearDamping (0.0f)
{
}


PhysicsMaterial::PhysicsMaterial (const Descriptor& desc)
	: m_angularDamping (desc.angularDamping),
	m_bounciness (desc.bounciness),
	m_friction (desc.friction),
	m_linearDamping (desc.linearDamping)
{
}


inline float PhysicsMaterial::GetAngularDamping () const
{
	return m_angularDamping;
}


inline float PhysicsMaterial::GetBounciness () const
{
	return m_bounciness;
}


inline float PhysicsMaterial::GetFriction () const
{
	return m_friction;
}


inline float PhysicsMaterial::GetLinearDamping () const
{
	return m_linearDamping;
}


inline void PhysicsMaterial::SetAngularDamping (float newAngularDamping)
{
	m_angularDamping = newAngularDamping;
}


inline void PhysicsMaterial::SetBounciness (float newBounciness)
{
	m_bounciness = newBounciness;
}


inline void PhysicsMaterial::SetFriction (float newFriction)
{
	m_friction = newFriction;
}


inline void PhysicsMaterial::SetLinearDamping (float newLinearDamping)
{
	m_linearDamping = newLinearDamping;
}

}	// namespace Engine
