#pragma once

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

// ---------------------------------- includes ----------------------------------

#include "btBulletDynamicsCommon.h"

#include "Component.h"
#include "PhysicsMaterial.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "Delegate.h"


namespace Engine {

// ----------------------------- forward declaration -----------------------------

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

	struct ShapeDescriptor
	{
		ShapeDescriptor ();

		std::string shapeType;
		float shapeDimensions[4];
		Ogre::Vector3 shapePos;
		Ogre::Quaternion shapeRot;
	};

	struct Descriptor
	{
		Descriptor ();

		std::string						name;
		float							mass;
		
		bool							isTrigger;
		std::string						rigidBodyType;
		Ogre::Vector3					angularFactor;
		std::vector<ShapeDescriptor>	shapeDescriptors;
		PhysicsMaterial::Descriptor		materialDescriptor;
	};

						PhysicsComponent (const std::string& name, float m);
						PhysicsComponent (const Descriptor& desc);

	void				Start () override;
	void				Update (float t, float dt) override;
	void				Destroy () override;

	void				ApplyDescriptor (const Descriptor& desc);
	void				UseDescriptor ();
	void				ApplyShapeDescriptors ();

	void				AddCollisionShape (btCollisionShape* collShape, const Ogre::Vector3& p, const Ogre::Quaternion& q);
	void				RemoveCollisionShape (btCollisionShape* collShape);
	void				CreateRigidBody ();

	DLL_EXPORT void		AddForce (float fx, float fy, float fz);
	DLL_EXPORT void		AddTorque (float tx, float ty, float tz);
	DLL_EXPORT void		SetAngularFactor (float x, float y, float z);
	DLL_EXPORT void		SetLinearVelocity (float x, float y, float z);
	DLL_EXPORT void		SetAngularVelocity (float x, float y, float z);
	DLL_EXPORT void		ActivateRigidBody ();

	DLL_EXPORT bool		IsTrigger () const;
	bool				IsDynamic () const;
	bool				IsKinematic () const;
	bool				IsStatic () const;

	void				SetMass ();
	void				SetTrigger (bool trigger);

	void				SetTypeToDynamic ();
	DLL_EXPORT void		SetTypeToKinematic ();
	void				SetTypeToStatic ();

	void				SetPhysicsMaterial (const PhysicsMaterial& phyMat);

private:
	RigidBodyType 			m_rigidBodyType;
	float					m_mass;
	bool					m_isTrigger;
	Descriptor				m_descriptor;

	btRigidBody*			m_pRigidBody;
	btCompoundShape*		m_pCompoundShape;
	btDefaultMotionState*	m_pMotionState;
	btDynamicsWorld*		m_pWorld;
	PhysicsMaterial*		m_pPhyMaterial;


	Ogre::Vector3		GetPosition () const;
	Ogre::Quaternion	GetOrientation () const;
	void				SetPosition (const Ogre::Vector3& p);
	void				SetOrientation (const Ogre::Quaternion& q);
};

}	// namespace Engine

#endif	// #ifndef PHYSICS_COMPONENT_H
