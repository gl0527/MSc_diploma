#include "WeaponComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "OgreEntity.h"
#include "OgreSkeletonInstance.h"
#include "OgreBone.h"


WeaponComponent::WeaponComponent (const std::string& name)
	: Component (name)
{
}

WeaponComponent::~WeaponComponent ()
{
}

void WeaponComponent::Start ()
{
}

void WeaponComponent::PreUpdate (float t, float dt)
{
	if (auto parent = m_owner->GetParent ().lock ()) {
		if (auto parentMesh = parent->GetFirstComponentByType<MeshComponent> ().lock ()) {
			if (auto parentSkeleton = parentMesh->GetEntity ()->getSkeleton ()) {
				if (auto rightHand = parentSkeleton->getBone ("hand_r")) {
					m_owner->Transform ()->SetLocalPosition (rightHand->_getDerivedPosition ());
					m_owner->Transform ()->SetLocalRotation (rightHand->_getDerivedOrientation ());
				}
			}
		}
	}
}
