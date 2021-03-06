#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "OgreVector3.h"


// using anonymous namespace to force internal linkage
// so its content will be accessible for only one translation unit (created from this source file)
namespace {

bool CollisionCallback (btManifoldPoint& /*cp*/, void* body0, void* body1)
{
	btRigidBody* rigidbody0 = static_cast<btRigidBody*> (body0);
	btRigidBody* rigidbody1 = static_cast<btRigidBody*> (body1);

	Engine::PhysicsComponent* collider0 = static_cast<Engine::PhysicsComponent*> (rigidbody0->getUserPointer ());
	Engine::PhysicsComponent* collider1 = static_cast<Engine::PhysicsComponent*> (rigidbody1->getUserPointer ());

	if (collider0 && collider0->IsTrigger ()) { // az elso szereplo trigger volt
		collider0->onTriggerEnter (collider1);
	}
	else if (collider1 && collider1->IsTrigger ()) { // a masodik szereplo trigger volt
		collider1->onTriggerEnter (collider0);
	}
	else {
		if (collider0)
			collider0->onCollision (collider1);

		if (collider1)
			collider1->onCollision (collider0);
	}

	return true;
}

}	// namespace


namespace Engine {

PhysicsSystem::PhysicsSystem ()
	: m_pCollisionConfig (nullptr),
	m_pDispatcher (nullptr),
	m_pOverlapPairCache (nullptr),
	m_pConstraintSolver (nullptr),
	m_pPhyWorld (nullptr)
{
}


PhysicsSystem& PhysicsSystem::GetInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (new PhysicsSystem); });
	return *s_pInstance.get ();
}


void PhysicsSystem::Init ()
{
	m_pCollisionConfig = new btDefaultCollisionConfiguration;
	m_pDispatcher = new btCollisionDispatcher (m_pCollisionConfig);
	m_pOverlapPairCache = new btDbvtBroadphase;
	m_pConstraintSolver = new btSequentialImpulseConstraintSolver;

	m_pPhyWorld = new btDiscreteDynamicsWorld (m_pDispatcher, m_pOverlapPairCache, m_pConstraintSolver, m_pCollisionConfig);
	m_pPhyWorld->setGravity (btVector3 (0.0f, -100.0f, 0.0f));
	gContactProcessedCallback = CollisionCallback;
}


bool PhysicsSystem::Update (float /*t*/, float dt)
{
	if (m_pPhyWorld != nullptr) {
		m_pPhyWorld->stepSimulation (dt);

		return true;
	} else {
		return false;
	}
}


void PhysicsSystem::Destroy ()
{
	if (m_pPhyWorld != nullptr) {
		delete m_pPhyWorld;
		m_pPhyWorld = nullptr;
	}
	if (m_pConstraintSolver != nullptr) {
		delete m_pConstraintSolver;
		m_pConstraintSolver = nullptr;
	}
	if (m_pOverlapPairCache != nullptr) {
		delete m_pOverlapPairCache;
		m_pOverlapPairCache = nullptr;
	}
	if (m_pDispatcher != nullptr) {
		delete m_pDispatcher;
		m_pDispatcher = nullptr;
	}
	if (m_pCollisionConfig != nullptr) {
		delete m_pCollisionConfig;
		m_pCollisionConfig = nullptr;
	}
}


btDiscreteDynamicsWorld* PhysicsSystem::GetWorldPtr () const
{
	return m_pPhyWorld;
}


btCollisionWorld::ClosestRayResultCallback PhysicsSystem::FirstHitRayCasting (const btVector3& from, const btVector3& to) const
{
	btCollisionWorld::ClosestRayResultCallback ray (from, to);
	
	if (m_pPhyWorld != nullptr)
		m_pPhyWorld->rayTest (from, to, ray);
	
	return ray;
}


btCollisionWorld::ClosestRayResultCallback PhysicsSystem::FirstHitRayCasting (const Ogre::Vector3& from, const Ogre::Vector3& to) const
{
	btVector3 rayStart{ from.x, from.y, from.z };
	btVector3 rayEnd{ to.x, to.y, to.z };

	btCollisionWorld::ClosestRayResultCallback ray (rayStart, rayEnd);

	if (m_pPhyWorld != nullptr)
		m_pPhyWorld->rayTest (rayStart, rayEnd, ray);

	return ray;
}


void PhysicsSystem::SetGravity (float y)
{
	btVector3 g (0.0f, y, 0.0f);
	if (m_pPhyWorld != nullptr)
		m_pPhyWorld->setGravity (g);
}


void PhysicsSystem::SetGravity (float x, float y, float z)
{
	btVector3 g (x, y, z);
	if (m_pPhyWorld != nullptr)
		m_pPhyWorld->setGravity (g);
}

}	// namespace Engine
