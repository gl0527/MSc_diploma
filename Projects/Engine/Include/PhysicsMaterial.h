#pragma once

#ifndef PHYSICS_MATERIAL_H
#define PHYSICS_MATERIAL_H

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"


namespace Engine {

// ============================ class PhysicsMaterial ============================

class PhysicsMaterial
{
public:
	struct Descriptor
	{
		Descriptor ();

		float angularDamping;
		float bounciness;
		float friction;
		float linearDamping;
	};

						PhysicsMaterial ();
						PhysicsMaterial (const Descriptor& desc);

	DLL_EXPORT float	GetAngularDamping () const;
	DLL_EXPORT float	GetBounciness () const;
	DLL_EXPORT float	GetFriction () const;
	DLL_EXPORT float	GetLinearDamping () const;

	DLL_EXPORT void		SetAngularDamping (float newAngularDamping);
	DLL_EXPORT void		SetBounciness (float newBounciness);
	DLL_EXPORT void		SetFriction (float newFriction);
	DLL_EXPORT void		SetLinearDamping (float newLinearDamping);

private:
	float m_angularDamping; // perdulet csillapitas
	float m_bounciness; // pattogossag
	float m_friction;	// surlodas
	float m_linearDamping; // linearis csillapitas
};

}	// namespace Engine

#endif	// #ifndef PHYSICS_MATERIAL_H