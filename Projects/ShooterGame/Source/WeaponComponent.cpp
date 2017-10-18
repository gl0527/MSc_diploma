#include "WeaponComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

WeaponComponent::WeaponComponent(const std::string& name)
	: Component(name)
{
}

WeaponComponent::~WeaponComponent()
{
}

void WeaponComponent::Start()
{
}

void WeaponComponent::PreUpdate(float t, float dt)
{
	if (auto& parent = m_owner->GetParent().lock())
	{
		if (auto parentMesh = parent->getFirstComponentByType<MeshComponent>().lock())
		{
			if (auto parentSkeleton = parentMesh->getEntity()->getSkeleton())
			{
				if (auto rightHand = parentSkeleton->getBone("hand_r"))
				{
					m_owner->Transform()->setPosition(rightHand->_getDerivedPosition());
					m_owner->Transform()->setRotation(rightHand->_getDerivedOrientation());
				}
			}
		}
	}
}
