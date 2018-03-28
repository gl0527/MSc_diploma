#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btVector3.h"
#include "System.h"


class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;


namespace Engine {

// ============================ class PhysicsSystem =============================

class PhysicsSystem : public System
{
public:
												PhysicsSystem ();

	virtual bool								init () override;
	virtual bool								update (float t, float dt) override;
	virtual void								destroy () override;

	DLL_EXPORT btDiscreteDynamicsWorld*			GetWorldPtr () const;
	btCollisionWorld::ClosestRayResultCallback	RayTest (const btVector3& from, const btVector3& to) const;

	void										SetGravity (float y);
	void										SetGravity (float x, float y, float z);

private:
	btDefaultCollisionConfiguration*		m_pCollisionConfig;
	btCollisionDispatcher*					m_pDispatcher;
	btBroadphaseInterface*					m_pOverlapPairCache;
	btSequentialImpulseConstraintSolver*	m_pConstraintSolver;
	btDiscreteDynamicsWorld*				m_pPhyWorld;
};

}	// namespace Engine

#endif	// #ifndef PHYSICS_SYSTEM_H
