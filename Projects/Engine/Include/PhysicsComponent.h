#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "btBulletDynamicsCommon.h"

#include "Component.h"
#include "Delegate.h"

// ----------------------------- forward declaration -----------------------------

namespace Ogre {

class Vector3;
class Quaternion;

}


namespace Engine {

class PhysicsMaterial;

// ============================ class PhysicsComponent ============================

class PhysicsComponent : public Component
{
private:
	enum class RigidBodyType : unsigned char
	{
		Static,
		Dynamic,
		Kinematic
	};

public:
	Delegate<void, PhysicsComponent*> onTriggerEnter;
	Delegate<void, PhysicsComponent*> onCollision;

	struct Descriptor
	{
		Descriptor ()
			: name (""),
			mass (0.0f),
			isTrigger (false),
			rigidBodyType (RigidBodyType::Static)
		{
		}

		std::string		name;
		float			mass;
		
		bool			isTrigger;
		RigidBodyType	rigidBodyType;
		void*			materialDesc;
	};

						PhysicsComponent (const std::string& name, float m);
						PhysicsComponent (const Descriptor& desc);
	virtual				~PhysicsComponent ();

	virtual void		PostInit (GameObject* object) override;
	virtual void		Update (float t, float dt) override;
	virtual void		Destroy () override;

	void				ApplyDescriptor (const Descriptor& desc);

	void				AddCollisionShape (btCollisionShape* collShape, const Ogre::Vector3& p, const Ogre::Quaternion& q);
	void				RemoveCollisionShape (btCollisionShape* collShape);
	void				CreateRigidBody ();

	DLL_EXPORT void		AddForce (float fx, float fy, float fz);
	DLL_EXPORT void		AddTorque (float tx, float ty, float tz);
	DLL_EXPORT void		SetAngularFactor (float x, float y, float z);
	DLL_EXPORT void		SetLinearVelocity (float x, float y, float z);
	DLL_EXPORT void		ActivateRigidBody ();
	void				DisableRotationXYZ ();

	DLL_EXPORT bool		IsTrigger () const;
	bool				IsDynamic () const;
	bool				IsKinematic () const;
	bool				IsStatic () const;

	void				SetMass ();
	void				SetTrigger (bool trigger);

	void				SetTypeToDynamic ();
	void				SetTypeToKinematic ();
	void				SetTypeToStatic ();

	void				SetPhysicsMaterial (const PhysicsMaterial& phyMat);

private:
	RigidBodyType 			m_rigidBodyType;

	float					m_mass;
	bool					m_isTrigger;

	btRigidBody*			m_pRigidBody;
	btCompoundShape*		m_pCompoundShape;
	btDefaultMotionState*	m_pMotionState;
	btDynamicsWorld*		m_pWorld;
	PhysicsMaterial*		m_pPhyMaterial;

	Ogre::Vector3		GetPosition ()		const;
	Ogre::Quaternion	GetOrientation ()	const;

	void				SetPosition (const Ogre::Vector3& p);
	void				SetOrientation (const Ogre::Quaternion& q);
};

}	// namespace Engine

#endif	// #ifndef PHYSICS_COMPONENT_H
