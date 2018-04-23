#include "SoldierAnimationComponent.h"
#include "InputManager.h"
#include "MeshComponent.h"
#include "GameObject.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonInstance.h"
#include "OgreEntity.h"


SoldierAnimationComponent::SoldierAnimationComponent (const std::string& name):
	Component (name),
	m_upperBodyAnimation (UpperBodyState::Idle),
	m_lowerBodyAnimation (LowerBodyState::Idle)
{
	m_upperBodyAnimation.AddTransitions ({
		{UpperBodyState::Idle,		UpperBodyState::Run,	'r', nullptr},
		{UpperBodyState::Idle,		UpperBodyState::Shoot,	's', nullptr},
		{UpperBodyState::Run,		UpperBodyState::Idle,	'i', nullptr},
		{UpperBodyState::Run,		UpperBodyState::Shoot,	's', nullptr},
		{UpperBodyState::Shoot,		UpperBodyState::Idle,	'i', nullptr},
		{UpperBodyState::Shoot,		UpperBodyState::Run,	'r', nullptr},
	});

	m_lowerBodyAnimation.AddTransitions ({
		{LowerBodyState::Idle,	LowerBodyState::Run,	'r', nullptr},
		{LowerBodyState::Run,	LowerBodyState::Idle,	'i', nullptr},
	});
}


void SoldierAnimationComponent::Start ()
{
	if (auto ownerMesh = m_owner->GetFirstComponentByType<MeshComponent> ().lock ()) {
		m_ownerEntity = ownerMesh->GetEntity ();

		if (auto skeleton = m_ownerEntity->getSkeleton ()) {
			skeleton->setBlendMode (Ogre::ANIMBLEND_CUMULATIVE);

			return;
		}
	}
	m_owner->RemoveComponent (m_name);
}


void SoldierAnimationComponent::PreUpdate (float t, float dt)
{
	if (InputManager::GetInstance ().IsKeyDown (OIS::KC_LSHIFT)) {
		m_upperBodyAnimation.Process ('r');
		m_lowerBodyAnimation.Process ('r');
	} else if (InputManager::GetInstance ().IsLeftMouseButtonDown ()) {
		m_upperBodyAnimation.Process ('s');
	} else {
		m_upperBodyAnimation.Process ('i');
		m_lowerBodyAnimation.Process ('i');
	}
	
	m_upperBodyAnimation.Update (t, dt);
	m_lowerBodyAnimation.Update (t, dt);
}