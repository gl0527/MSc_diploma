#include "CameraControlComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "PhysicsSystem.h"
#include "TransformComponent.h"


CameraControlComponent::CameraControlComponent(const std::string& name)
	: Component(name),
	camHeight(0.0f),
	targetHeight(0.0f),
	camDist(0.0f),
	motBlend(0.0f),
	fixed(false),
	world(Game::GetInstance().GetPhysicsSystem()),
	ownerCam(std::shared_ptr<CameraComponent>(nullptr))
{
}


CameraControlComponent::~CameraControlComponent()
{
}


void CameraControlComponent::Start()
{
	ownerCam = m_owner->GetFirstComponentByType<CameraComponent>();
	if (ownerCam.lock() == nullptr)
		m_owner->RemoveComponent(m_name);
}


void CameraControlComponent::PostUpdate(float t, float dt)
{
	if (auto cam = ownerCam.lock())
	{
		if (auto parent = m_owner->GetParent().lock())
		{
			//const auto& ownerPos = m_owner->Transform()->GetPositionInWorldSpace();
			//const auto& parentPos = parent->Transform()->GetPositionInWorldSpace();

			/*btVector3 btOwnerPos(ownerPos.x, ownerPos.y, ownerPos.z);
			btVector3 btParentPos(parentPos.x, parentPos.y, parentPos.z);

			auto& ray = world->rayTest(btParentPos, btParentPos);

			if (ray.hasHit())
			{
				std::cout << "hit\n";
			}*/
		}
	}
}
