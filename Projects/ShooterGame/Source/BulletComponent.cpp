#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "PlayerDataComponent.h"
#include "ObjectManager.h"


BulletComponent::BulletComponent (const std::string& name) :
	Component (name)
{
}


void BulletComponent::Start ()
{
	if (auto ownerPhysics = m_owner->GetFirstComponentByType<PhysicsComponent> ().lock ()) {
		ownerPhysics->onCollision += std::bind (&BulletComponent::OnCollisionWithEnemy, this, std::placeholders::_1);
	} else {
		m_owner->RemoveComponent (m_name);
	}
}


void BulletComponent::OnCollisionWithEnemy (PhysicsComponent* other)
{
	GameObject* otherOwner = other->GetOwner ();
	
	if (!otherOwner->HasTag ("enemy")) {
		ObjectManager::GetInstance ().MarkGameObjectForDelete (m_owner->GetName ());
		return;
	}
	if (auto otherPlayerData = otherOwner->GetFirstComponentByType<PlayerDataComponent> ().lock ())
		otherPlayerData->DecreaseHealtPoint (1);

	ObjectManager::GetInstance ().MarkGameObjectForDelete (m_owner->GetName ());
}
