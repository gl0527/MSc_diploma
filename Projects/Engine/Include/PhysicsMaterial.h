#ifndef PHYSICS_MATERIAL_H
#define PHYSICS_MATERIAL_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"


namespace Engine
{

// ============================ class PhysicsMaterial ============================

class DLL_SPEC PhysicsMaterial
{
public:
	PhysicsMaterial ();

	float	GetAngularDamping () const;
	float	GetBounciness () const;
	float	GetFriction () const;
	float	GetLinearDamping () const;

	void	SetAngularDamping (float newAngularDamping);
	void	SetBounciness (float newBounciness);
	void	SetFriction (float newFriction);
	void	SetLinearDamping (float newLinearDamping);

private:
	float m_angularDamping; // perdulet csillapitas
	float m_bounciness; // pattogossag
	float m_friction;	// surlodas
	float m_linearDamping; // linearis csillapitas
};

}	// namespace Engine

#endif	// #ifndef PHYSICS_MATERIAL_H