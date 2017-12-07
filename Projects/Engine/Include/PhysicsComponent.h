#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include <functional>

#include "btBulletDynamicsCommon.h"

#include "Component.h"
#include "PhysicsMaterial.h"

// ----------------------------- forward declaration -----------------------------

namespace Ogre {
class Vector3;
class Quaternion;
}


namespace Engine {

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
	struct Descriptor
	{
		Descriptor ()
			: name (""),
			mass (0.0f),
			isTrigger (false),
			rigidBodyType (RigidBodyType::Static)
		{
		}

		std::string name;
		float mass;
		
		bool isTrigger;
		RigidBodyType rigidBodyType;
		PhysicsMaterial::Descriptor materialDesc;
	};

	PhysicsComponent (const std::string& name, float m);
	PhysicsComponent (const Descriptor& desc);
	virtual ~PhysicsComponent ();

	virtual void PostInit (GameObject* object) override;
	virtual void Update (float t, float dt) override;
	virtual void Destroy () override;

	void ApplyDescriptor (const Descriptor& desc);

	void AddCollisionShape (btCollisionShape* collShape, const Ogre::Vector3& p, const Ogre::Quaternion& q);
	void RemoveCollisionShape (btCollisionShape* collShape);
	void CreateRigidBody ();

	DLL_EXPORT void AddForce (float fx, float fy, float fz);
	void SetAngularFactor (float x, float y, float z);
	DLL_EXPORT void SetLinearVelocity (float x, float y, float z);
	DLL_EXPORT void ActivateRigidBody ();
	void DisableRotationXYZ ();

	DLL_EXPORT bool IsTrigger () const;
	bool IsDynamic () const;
	bool IsKinematic () const;
	bool IsStatic () const;

	void SetMass ();
	void SetTrigger (bool trigger);

	void SetTypeToDynamic ();
	void SetTypeToKinematic ();
	void SetTypeToStatic ();

	void SetPhysicsMaterial (const PhysicsMaterial& phyMat);

	void setOnTriggerEnter (std::function<void (PhysicsComponent*)>&& f) { triggerEnter = f; }
	void setOnCollision (std::function<void (PhysicsComponent*)>&& f) { collision = f; }

	void onTriggerEnter (PhysicsComponent* other) { triggerEnter (other); }
	void onCollision (PhysicsComponent* other) { collision (other); }

private:
	RigidBodyType m_rigidBodyType;

	float	m_mass;
	bool	m_isTrigger;

	btRigidBody*			m_pRigidBody;
	btCompoundShape*		m_pCompoundShape;
	btDefaultMotionState*	m_pMotionState;
	btDynamicsWorld*		m_pWorld;
	PhysicsMaterial			m_physicsMaterial;


	Ogre::Vector3		GetPosition ()		const;
	Ogre::Quaternion	GetOrientation ()	const;

	void SetPosition (const Ogre::Vector3& p);
	void SetOrientation (const Ogre::Quaternion& q);

	std::function<void (PhysicsComponent*)> triggerEnter;
	std::function<void (PhysicsComponent*)> collision;

	static void defaultTriggerEnter (PhysicsComponent* /*other*/) {}
	static void defaultCollision (PhysicsComponent* /*other*/) {}
};

}	// namespace Engine

#endif	// #ifndef PHYSICS_COMPONENT_H
