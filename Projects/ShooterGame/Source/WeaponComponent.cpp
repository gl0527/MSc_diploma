#include "WeaponComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "OgreEntity.h"
#include "OgreSkeletonInstance.h"
#include "OgreBone.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "PlayerDataComponent.h"
#include "AudioSourceComponent.h"


WeaponComponent::WeaponComponent (const std::string& name)
	: Component (name),
	m_FullAmmo (15),
	m_ammo (10),
	m_pOwnerAudio (nullptr)
{
}


void WeaponComponent::Start ()
{
	if (auto ownerAudio = m_owner->GetFirstComponentByType<AudioSourceComponent> ().lock ())
		m_pOwnerAudio = ownerAudio;

	if (m_pOwnerAudio == nullptr)
		m_owner->RemoveComponent (m_name);
}


void WeaponComponent::PreUpdate (float t, float dt)
{
	if (auto parent = m_owner->GetParent ().lock ()) {
		if (auto parentMesh = parent->GetFirstComponentByType<MeshComponent> ().lock ()) {
			if (auto parentSkeleton = parentMesh->GetEntity ()->getSkeleton ()) {
				if (auto rightHand = parentSkeleton->getBone ("hand_r")) {
					m_owner->Transform ()->SetLocalPosition (rightHand->_getDerivedPosition ());
					Ogre::Quaternion rotation (Ogre::Radian (Ogre::Math::PI * 0.5f), Ogre::Vector3::UNIT_X);
					m_owner->Transform ()->SetLocalRotation (rightHand->_getDerivedOrientation () * rotation);
				}
			}
		}
	}
}


unsigned char WeaponComponent::GetAmmo () const
{
	return m_ammo;
}


void WeaponComponent::SetAmmoToFull ()
{
	m_ammo = m_FullAmmo;

	if (m_pOwnerAudio != nullptr)
		m_pOwnerAudio->Play ("i-dont-think-so-2.wav");
}


bool WeaponComponent::HasAmmo () const
{
	return m_ammo > 0;
}


void WeaponComponent::Shoot ()
{
	if (m_ammo == 0)
		return;

	--m_ammo;

	Ogre::Vector3 rayStart = m_owner->Transform ()->GetGlobalPosition () + m_owner->Transform ()->GetGlobalFacing () * 5.0f;
	Ogre::Vector3 rayEnd = m_owner->Transform ()->GetGlobalPosition () + m_owner->Transform ()->GetGlobalFacing () * 500.0f;

	auto ray = PhysicsSystem::GetInstance ().FirstHitRayCasting (rayStart, rayEnd);

	if (ray.hasHit ()) {
		PhysicsComponent* hitPhysics = reinterpret_cast<PhysicsComponent*> (ray.m_collisionObject->getUserPointer ());
		GameObject* hitObject = hitPhysics->GetOwner ();

		if (hitObject->HasTag ("enemy")) {
			if (auto hitData = hitObject->GetFirstComponentByType<PlayerDataComponent> ().lock ())
				hitData->DecreaseHealtPoint (1);
		}
	}

	if (m_pOwnerAudio != nullptr)
		m_pOwnerAudio->Play ("Single rifle shot.wav");
}