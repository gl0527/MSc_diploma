#include "WeaponComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "OgreEntity.h"
#include "OgreSkeletonInstance.h"
#include "OgreBone.h"


WeaponComponent::WeaponComponent (const std::string& name)
	: Component (name),
	m_FullAmmo (15),
	m_ammo (10)
{
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


void WeaponComponent::DecreaseAmmoByOne ()
{
	if (m_ammo > 0)
		--m_ammo;
}


void WeaponComponent::SetAmmoToFull ()
{
	m_ammo = m_FullAmmo;
}


bool WeaponComponent::HasAmmo () const
{
	return m_ammo > 0;
}
