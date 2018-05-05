#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "PhysicsSystem.h"
#include "TransformComponent.h"


namespace Engine {

PhysicsComponent::ShapeDescriptor::ShapeDescriptor () :
	shapeType (""),
	shapePos (Ogre::Vector3::ZERO),
	shapeRot (Ogre::Quaternion::IDENTITY)
{
	for (unsigned char i = 0; i < 4; ++i)
		shapeDimensions[i] = 0.0f;
}


PhysicsComponent::Descriptor::Descriptor () :
	name (""),
	mass (0.0f),
	isTrigger (false),
	rigidBodyType (""),
	angularFactor (Ogre::Vector3 (1.0f, 1.0f, 1.0f))
{
}


PhysicsComponent::PhysicsComponent (const std::string& name, float m)
	: Component (name, true),
	m_mass (m),
	m_isTrigger (false),
	m_pRigidBody (nullptr),
	m_pCompoundShape (new btCompoundShape),
	m_pWorld (Game::GetInstance ().GetPhysicsSystem ()->GetWorldPtr ()),
	m_pPhyMaterial (nullptr)
{
	if (m_mass < 1e-4) {
		m_mass = 0.0f;
		m_rigidBodyType = RigidBodyType::Static;
	} else
		m_rigidBodyType = RigidBodyType::Dynamic;
}


PhysicsComponent::PhysicsComponent (const Descriptor& desc)
	: PhysicsComponent (desc.name, desc.mass)
{
}


void PhysicsComponent::AddCollisionShape (btCollisionShape* collShape, const Ogre::Vector3& p, const Ogre::Quaternion& q)
{
	btTransform pose (btQuaternion (q.x, q.y, q.z, q.w), btVector3 (p.x, p.y, p.z));
	m_pCompoundShape->addChildShape (pose, collShape);
}


void PhysicsComponent::RemoveCollisionShape (btCollisionShape* collShape)
{
	m_pCompoundShape->removeChildShape (collShape);
}


void PhysicsComponent::Start ()
{
	ApplyShapeDescriptors ();
	SetMass ();
	CreateRigidBody ();
	UseDescriptor ();
}


void PhysicsComponent::Update (float t, float dt)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic) {
		m_owner->Transform ()->SetGlobalPosition (GetPosition ());
		m_owner->Transform ()->SetGlobalRotation (GetOrientation ());
	} else if (m_rigidBodyType == RigidBodyType::Kinematic) {
		SetPosition (m_owner->Transform ()->GetGlobalPosition ());
		SetOrientation (m_owner->Transform ()->GetGlobalRotation ());
	}
}


void PhysicsComponent::Destroy ()
{
	for (int i = 0; i < m_pCompoundShape->getNumChildShapes (); ++i) {
		btCollisionShape* s = m_pCompoundShape->getChildShape (i);
		m_pCompoundShape->removeChildShape (s);
	}
	btCollisionObject* obj = static_cast<btCollisionObject*>(m_pRigidBody);
	m_pWorld->removeCollisionObject (obj);
	m_pWorld->removeRigidBody (m_pRigidBody);

	if (m_pCompoundShape != nullptr) {
		delete m_pCompoundShape;
		m_pCompoundShape = nullptr;
	}
	if (m_pMotionState != nullptr) {
		delete m_pMotionState;
		m_pMotionState = nullptr;
	}
	if (m_pRigidBody != nullptr) {
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}
}


void PhysicsComponent::ApplyDescriptor (const PhysicsComponent::Descriptor& desc)
{
	m_descriptor = desc;
}


void PhysicsComponent::ApplyShapeDescriptors ()
{
	for (const ShapeDescriptor& shapeDesc : m_descriptor.shapeDescriptors) {
		std::string shapeType = shapeDesc.shapeType;
		float x = shapeDesc.shapeDimensions[0];
		float y = shapeDesc.shapeDimensions[1];
		float z = shapeDesc.shapeDimensions[2];
		float w = shapeDesc.shapeDimensions[3];
		btCollisionShape* pCollisionShape = nullptr;

		if (shapeType == "box") {
			pCollisionShape = new btBoxShape (btVector3 (x * 0.5f, y * 0.5f, z * 0.5f));
		} else if (shapeType == "staticplane") {
			pCollisionShape = new btStaticPlaneShape (btVector3 (x, y, z), w);
		} else if (shapeType == "capsule") {
			pCollisionShape = new btCapsuleShape (x, y);
		} else if (shapeType == "sphere") {
			pCollisionShape = new btSphereShape (x);
		}

		if (pCollisionShape != nullptr)
			AddCollisionShape (pCollisionShape, shapeDesc.shapePos, shapeDesc.shapeRot);
	}
}


void PhysicsComponent::UseDescriptor ()
{
	SetTrigger (m_descriptor.isTrigger);

	if (m_descriptor.rigidBodyType == "dynamic")
		m_rigidBodyType = RigidBodyType::Dynamic;
	else if (m_descriptor.rigidBodyType == "kinematic")
		m_rigidBodyType = RigidBodyType::Kinematic;
	else
		m_rigidBodyType = RigidBodyType::Static;

	SetAngularFactor (m_descriptor.angularFactor.x, m_descriptor.angularFactor.y, m_descriptor.angularFactor.z);

	PhysicsMaterial physicsMaterial;

	physicsMaterial.SetFriction (m_descriptor.materialDescriptor.friction);
	physicsMaterial.SetLinearDamping (m_descriptor.materialDescriptor.linearDamping);
	physicsMaterial.SetAngularDamping (m_descriptor.materialDescriptor.angularDamping);
	physicsMaterial.SetBounciness (m_descriptor.materialDescriptor.bounciness);

	SetPhysicsMaterial (physicsMaterial);
}


void PhysicsComponent::CreateRigidBody ()
{
	if (m_pRigidBody != nullptr) {
		m_pWorld->removeRigidBody (m_pRigidBody);
		m_pRigidBody->setUserPointer (nullptr);
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}

	const auto& q = m_owner->Transform ()->GetGlobalRotation ();
	const auto& p = m_owner->Transform ()->GetGlobalPosition ();

	btQuaternion physicsRot (q.x, q.y, q.z, q.w);
	btVector3 physicsPos (p.x, p.y, p.z);

	btTransform pose (physicsRot, physicsPos);
	m_pMotionState = new btDefaultMotionState (pose);

	btVector3 inertia (0, 0, 0);
	if (m_rigidBodyType == RigidBodyType::Static)
		m_mass = 0;
	else
		m_pCompoundShape->calculateLocalInertia (m_mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI (m_mass, m_pMotionState, m_pCompoundShape, inertia);
	m_pRigidBody = new btRigidBody (rigidBodyCI);
	m_pRigidBody->setMassProps (m_mass, inertia);

	if (m_rigidBodyType == RigidBodyType::Kinematic) {
		m_pRigidBody->setCollisionFlags (m_pRigidBody->getCollisionFlags () | btCollisionObject::CF_KINEMATIC_OBJECT);
		m_pRigidBody->setActivationState (DISABLE_DEACTIVATION);
	}
	m_pRigidBody->setUserPointer (this);
	m_pWorld->addRigidBody (m_pRigidBody);

	btBroadphaseProxy* broadPhaseProxy = m_pRigidBody->getBroadphaseHandle ();
	if (broadPhaseProxy != nullptr) {
		broadPhaseProxy->m_collisionFilterGroup = static_cast<short> (btBroadphaseProxy::DefaultFilter);
		broadPhaseProxy->m_collisionFilterMask = static_cast<short> (btBroadphaseProxy::AllFilter);
	}
}


Ogre::Vector3 PhysicsComponent::GetPosition () const
{
	btTransform transform;
	m_pMotionState->getWorldTransform (transform);
	return Ogre::Vector3 (transform.getOrigin ().x (), transform.getOrigin ().y (), transform.getOrigin ().z ());
}


Ogre::Quaternion PhysicsComponent::GetOrientation () const
{
	btTransform t;
	m_pMotionState->getWorldTransform (t);
	return Ogre::Quaternion (t.getRotation ().w (), t.getRotation ().x (), t.getRotation ().y (), t.getRotation ().z ());
}


void PhysicsComponent::SetPosition (const Ogre::Vector3& p)
{
	btTransform prevTransform;
	m_pMotionState->getWorldTransform (prevTransform);
	btTransform newTransform (prevTransform.getRotation (), btVector3 (p.x, p.y, p.z));
	m_pMotionState->setWorldTransform (newTransform);
}


void PhysicsComponent::SetOrientation (const Ogre::Quaternion& q)
{
	btTransform prevTransform;
	m_pMotionState->getWorldTransform (prevTransform);
	btTransform newTransform (btQuaternion (q.x, q.y, q.z, q.w), prevTransform.getOrigin ());
	m_pMotionState->setWorldTransform (newTransform);
}


void PhysicsComponent::SetPhysicsMaterial (const PhysicsMaterial& phyMat)
{
	if (m_pPhyMaterial != nullptr)
		delete m_pPhyMaterial;

	m_pPhyMaterial = new PhysicsMaterial (phyMat);

	if (m_pRigidBody != nullptr) {
		m_pRigidBody->setFriction (m_pPhyMaterial->GetFriction ());
		m_pRigidBody->setDamping (m_pPhyMaterial->GetLinearDamping (), m_pPhyMaterial->GetAngularDamping ());
		m_pRigidBody->setRestitution (m_pPhyMaterial->GetBounciness ());
	}
}


void PhysicsComponent::AddForce (float fx, float fy, float fz)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic && m_pRigidBody != nullptr) {
		btVector3 centralForce (fx, fy, fz);
		m_pRigidBody->applyCentralForce (centralForce);
	}
}


void PhysicsComponent::AddTorque (float tx, float ty, float tz)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic && m_pRigidBody != nullptr) {
		btVector3 torque (tx, ty, tz);
		m_pRigidBody->applyTorque (torque);
	}
}


void PhysicsComponent::SetAngularFactor (float x, float y, float z)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic && m_pRigidBody != nullptr)
		m_pRigidBody->setAngularFactor (btVector3 (x, y, z));
}


void PhysicsComponent::SetLinearVelocity (float x, float y, float z)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic && m_pRigidBody != nullptr)
		m_pRigidBody->setLinearVelocity (btVector3 (x, y, z));
}


void PhysicsComponent::SetAngularVelocity (float x, float y, float z)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic && m_pRigidBody != nullptr)
		m_pRigidBody->setAngularVelocity (btVector3 (x, y, z));
}


void PhysicsComponent::ActivateRigidBody ()
{
	if (m_pRigidBody != nullptr)
		m_pRigidBody->activate (true);
}


bool PhysicsComponent::IsTrigger () const
{
	return m_isTrigger;
}


bool PhysicsComponent::IsDynamic () const
{
	return m_rigidBodyType == RigidBodyType::Dynamic;
}


bool PhysicsComponent::IsKinematic () const
{
	return m_rigidBodyType == RigidBodyType::Kinematic;
}


bool PhysicsComponent::IsStatic () const
{
	return m_rigidBodyType == RigidBodyType::Static;
}


void PhysicsComponent::SetMass ()
{
	if (m_mass == 0 && m_rigidBodyType != RigidBodyType::Static)
		SetTypeToStatic ();

	if (m_mass > 0) {
		btVector3 inertia (0, 0, 0);
		m_pCompoundShape->calculateLocalInertia (m_mass, inertia);
	}
}


void PhysicsComponent::SetTrigger (bool t)
{
	m_isTrigger = t;
	if (m_isTrigger) {
		if (m_pRigidBody != nullptr)
			m_pRigidBody->setCollisionFlags (m_pRigidBody->getCollisionFlags () | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	} else {
		if (m_pRigidBody != nullptr)
			m_pRigidBody->setCollisionFlags (m_pRigidBody->getCollisionFlags () & !btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}


void PhysicsComponent::SetTypeToDynamic ()
{
	m_rigidBodyType = RigidBodyType::Dynamic;
	if (m_pRigidBody != nullptr)
		CreateRigidBody ();
}


void PhysicsComponent::SetTypeToKinematic ()
{
	m_rigidBodyType = RigidBodyType::Kinematic;
	if (m_pRigidBody != nullptr)
		CreateRigidBody ();
}


void PhysicsComponent::SetTypeToStatic ()
{
	m_rigidBodyType = RigidBodyType::Static;
	if (m_pRigidBody != nullptr)
		CreateRigidBody ();
}

}	// namespace Engine
