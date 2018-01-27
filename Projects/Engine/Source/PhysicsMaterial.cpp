#include "PhysicsMaterial.h"


namespace Engine {

PhysicsMaterial::Descriptor::Descriptor ()
	: angularDamping (0.0f),
	bounciness (0.0f),
	friction (0.0f),
	linearDamping (0.0f)
{
}


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


float PhysicsMaterial::GetAngularDamping () const
{
	return m_angularDamping;
}


float PhysicsMaterial::GetBounciness () const
{
	return m_bounciness;
}


float PhysicsMaterial::GetFriction () const
{
	return m_friction;
}


float PhysicsMaterial::GetLinearDamping () const
{
	return m_linearDamping;
}


void PhysicsMaterial::SetAngularDamping (float newAngularDamping)
{
	m_angularDamping = newAngularDamping;
}


void PhysicsMaterial::SetBounciness (float newBounciness)
{
	m_bounciness = newBounciness;
}


void PhysicsMaterial::SetFriction (float newFriction)
{
	m_friction = newFriction;
}


void PhysicsMaterial::SetLinearDamping (float newLinearDamping)
{
	m_linearDamping = newLinearDamping;
}

}	// namespace Engine
