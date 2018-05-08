#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btVector3.h"
#include "SingletonBase.h"


class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;


namespace Ogre {

class Vector3;

}	// namespace Ogre


namespace Engine {

// ============================ class PhysicsSystem =============================

class PhysicsSystem : public SingletonBase<PhysicsSystem>
{
public:
	static DLL_EXPORT PhysicsSystem&						GetInstance ();

	void													Init ();
	bool													Update (float t, float dt);
	void													Destroy ();

	DLL_EXPORT btDiscreteDynamicsWorld*						GetWorldPtr () const;
	btCollisionWorld::ClosestRayResultCallback				FirstHitRayCasting (const btVector3& from, const btVector3& to) const;
	DLL_EXPORT btCollisionWorld::ClosestRayResultCallback	FirstHitRayCasting (const Ogre::Vector3& from, const Ogre::Vector3& to) const;

	void													SetGravity (float y);
	void													SetGravity (float x, float y, float z);

private:
	friend class SingletonBase<PhysicsSystem>;

	btDefaultCollisionConfiguration*		m_pCollisionConfig;
	btCollisionDispatcher*					m_pDispatcher;
	btBroadphaseInterface*					m_pOverlapPairCache;
	btSequentialImpulseConstraintSolver*	m_pConstraintSolver;
	btDiscreteDynamicsWorld*				m_pPhyWorld;


	PhysicsSystem ();
};

}	// namespace Engine

#endif	// #ifndef PHYSICS_SYSTEM_H
