#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "PhysicsSystem.h"
#include <Ogre.h>
#include "TransformComponent.h"
#include "PhysicsMaterial.h"


namespace Engine {

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
	}
	else
		m_rigidBodyType = RigidBodyType::Dynamic;
}


PhysicsComponent::PhysicsComponent (const Descriptor& desc)
	: Component (desc.name),
	m_mass (desc.mass),
	m_isTrigger (desc.isTrigger),
	m_pRigidBody (nullptr),
	m_pCompoundShape (new btCompoundShape),
	m_pWorld (Game::GetInstance ().GetPhysicsSystem ()->GetWorldPtr ())
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


void PhysicsComponent::CreateRigidBody ()
{
	if (m_pRigidBody) {
		m_pWorld->removeRigidBody (m_pRigidBody);
		m_pRigidBody->setUserPointer (nullptr);
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}

	const auto& q = m_owner->Transform ()->GetRotationInWorldSpace ();
	const auto& p = m_owner->Transform ()->GetPositionInWorldSpace ();

	btTransform pose (btQuaternion (q.x, q.y, q.z, q.w), btVector3 (p.x, p.y, p.z));
	m_pMotionState = new btDefaultMotionState (pose);

	btVector3 inertia (0, 0, 0);
	if (m_rigidBodyType == RigidBodyType::Static)
		m_mass = 0.0f;
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
}


void PhysicsComponent::PostInit (GameObject* object)
{
	SetMass ();
	CreateRigidBody ();
}


void PhysicsComponent::Update (float t, float dt)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic) {
		m_owner->Transform ()->SetWorldPosition (GetPosition ());
		m_owner->Transform ()->SetWorldRotation (GetOrientation ());
	}
	else if (m_rigidBodyType == RigidBodyType::Kinematic) {
		SetPosition (m_owner->Transform ()->GetPositionInWorldSpace ());
		SetOrientation (m_owner->Transform ()->GetRotationInWorldSpace ());

		// getshape, atallitas (localscale), setshape
		// vagy rigidbody lezuzas

		/*auto collShape = rigidBody->getCollisionShape();
		const auto& ownerScale = ownerObject->transform()->scale();
		collShape->setLocalScaling(btVector3(ownerScale.x, ownerScale.y, ownerScale.z));
		rigidBody->setCollisionShape(collShape);*/
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

	if (m_pCompoundShape) {
		delete m_pCompoundShape;
		m_pCompoundShape = nullptr;
	}
	if (m_pMotionState) {
		delete m_pMotionState;
		m_pMotionState = nullptr;
	}
	if (m_pRigidBody) {
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}
}


void PhysicsComponent::ApplyDescriptor (const Descriptor& desc)
{

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

	if (m_pRigidBody) {
		m_pRigidBody->setFriction (m_pPhyMaterial->GetFriction ());
		m_pRigidBody->setDamping (m_pPhyMaterial->GetLinearDamping (), m_pPhyMaterial->GetAngularDamping ());
		m_pRigidBody->setRestitution (m_pPhyMaterial->GetBounciness ());
	}
}


void PhysicsComponent::AddForce (float fx, float fy, float fz)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic && m_pRigidBody) {
		btVector3 centralForce (fx, fy, fz);
		m_pRigidBody->applyCentralForce (centralForce);
	}
}


void PhysicsComponent::SetAngularFactor (float x, float y, float z)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic && m_pRigidBody)
		m_pRigidBody->setAngularFactor (btVector3 (x, y, z));
}


void PhysicsComponent::SetLinearVelocity (float x, float y, float z)
{
	if (m_rigidBodyType == RigidBodyType::Dynamic && m_pRigidBody)
		m_pRigidBody->setLinearVelocity (btVector3 (x, y, z));
}


void PhysicsComponent::ActivateRigidBody ()
{
	if (m_pRigidBody)
		m_pRigidBody->activate (true);
}


void PhysicsComponent::DisableRotationXYZ ()
{
	if (m_rigidBodyType == RigidBodyType::Dynamic && m_pRigidBody != nullptr)
		m_pRigidBody->setAngularFactor (0);
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
		if (m_pRigidBody)
			m_pRigidBody->setCollisionFlags (m_pRigidBody->getCollisionFlags () | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	else {
		if (m_pRigidBody)
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


PhysicsComponent::~PhysicsComponent ()
{
}

}	// namespace Engine
