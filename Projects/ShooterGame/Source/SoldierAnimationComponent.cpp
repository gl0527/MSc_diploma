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
	m_lowerBodyAnimation (LowerBodyState::Idle),
	m_ownerEntity (nullptr),
	m_isInShootState (false),
	m_hasWeapon (false)
{
	m_upperBodyAnimation.AddTransitions ({
		{UpperBodyState::Idle,			UpperBodyState::Run,		'r', [this] { OnTransition ("up_stand", "up_run", true); }},
		{UpperBodyState::Idle,			UpperBodyState::WeaponHold,	'w', [this] { OnTransition ("up_stand", "up_weapon_hold", true); }},
		{UpperBodyState::Run,			UpperBodyState::Idle,		'i', [this] { OnTransition ("up_run", "up_stand", true); }},
		{UpperBodyState::Run,			UpperBodyState::WeaponHold,	'w', [this] { OnTransition ("up_run", "up_weapon_hold", true); }},
		{UpperBodyState::WeaponHold,	UpperBodyState::Shoot,		's', [this] { OnTransition ("up_weapon_hold", "up_shoot", false); }},
		{UpperBodyState::Shoot,			UpperBodyState::WeaponHold,	'w', [this] { OnTransition ("up_shoot", "up_weapon_hold", true); }},
	});

	m_lowerBodyAnimation.AddTransitions ({
		{LowerBodyState::Idle,	LowerBodyState::Run,	'r', [this] { OnTransition ("leg_stand", "leg_run", true); }},
		{LowerBodyState::Run,	LowerBodyState::Idle,	'i', [this] { OnTransition ("leg_run", "leg_stand", true); }},
	});

	m_upperBodyAnimation.AddStateFunction (UpperBodyState::Idle,
		std::bind (&SoldierAnimationComponent::OnUpperBodyIdle, this, std::placeholders::_1, std::placeholders::_2));

	m_upperBodyAnimation.AddStateFunction (UpperBodyState::Run,
		std::bind (&SoldierAnimationComponent::OnUpperBodyRun, this, std::placeholders::_1, std::placeholders::_2));

	m_upperBodyAnimation.AddStateFunction (UpperBodyState::Shoot,
		std::bind (&SoldierAnimationComponent::OnUpperBodyShoot, this, std::placeholders::_1, std::placeholders::_2));

	m_upperBodyAnimation.AddStateFunction (UpperBodyState::WeaponHold,
		std::bind (&SoldierAnimationComponent::OnUpperBodyWeaponHold, this, std::placeholders::_1, std::placeholders::_2));

	m_lowerBodyAnimation.AddStateFunction (LowerBodyState::Idle,
		std::bind (&SoldierAnimationComponent::OnLowerBodyIdle, this, std::placeholders::_1, std::placeholders::_2));

	m_lowerBodyAnimation.AddStateFunction (LowerBodyState::Run,
		std::bind (&SoldierAnimationComponent::OnLowerBodyRun, this, std::placeholders::_1, std::placeholders::_2));
}


void SoldierAnimationComponent::Start ()
{
	if (auto ownerMesh = m_owner->GetFirstComponentByType<MeshComponent> ().lock ()) {
		m_ownerEntity =  (ownerMesh->GetEntity ());

		if (auto skeleton = m_ownerEntity->getSkeleton ()) {
			skeleton->setBlendMode (Ogre::ANIMBLEND_CUMULATIVE);

			return;
		}
	}
	m_owner->RemoveComponent (m_name);
}


void SoldierAnimationComponent::PreUpdate (float t, float dt)
{
	if (m_hasWeapon) {
		if ((m_isInShootState || InputManager::GetInstance ().IsLeftMouseButtonDown ())) {
			m_isInShootState = true;
			m_upperBodyAnimation.Process ('s');
		} else {
			m_upperBodyAnimation.Process ('w');
		}
		if (InputManager::GetInstance ().IsKeyDown (OIS::KC_W)) {
			m_lowerBodyAnimation.Process ('r');	// ezeknek csak akkor kene lefutniuk, amikor az esemeny bekovetkezik -> feliratkozas az inputmanagerre!!!
		} else {
			m_lowerBodyAnimation.Process ('i');
		}
	} else {
		if (InputManager::GetInstance ().IsKeyDown (OIS::KC_W)) {
			m_upperBodyAnimation.Process ('r');
			m_lowerBodyAnimation.Process ('r');
		} else {
			m_upperBodyAnimation.Process ('i');
			m_lowerBodyAnimation.Process ('i');
		}
	}
	
	m_upperBodyAnimation.Update (t, dt);
	m_lowerBodyAnimation.Update (t, dt);
}


void SoldierAnimationComponent::OnTransition (const char* fromAnimName, const char* toAnimName, bool isLooping)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState (fromAnimName);
	pAnimState->setEnabled (false);
	pAnimState->setWeight (0.0f);
	pAnimState->setLoop (false);

	pAnimState = m_ownerEntity->getAnimationState (toAnimName);
	pAnimState->setEnabled (true);
	pAnimState->setWeight (1.0f);
	pAnimState->setTimePosition (0.0f);
	pAnimState->setLoop (isLooping);
}


void SoldierAnimationComponent::OnUpperBodyIdle (float t, float dt)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState ("up_stand");
	pAnimState->setEnabled (true);
	pAnimState->setWeight (1.0f);
	pAnimState->setLoop (true);
	pAnimState->addTime (dt);
}


void SoldierAnimationComponent::OnUpperBodyRun (float t, float dt)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState ("up_run");
	pAnimState->addTime (dt);
}


void SoldierAnimationComponent::OnUpperBodyShoot (float t, float dt)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState ("up_shoot");
	pAnimState->addTime (dt);
	if (pAnimState->hasEnded ())
		m_isInShootState = false;
}


void SoldierAnimationComponent::OnUpperBodyWeaponHold (float t, float dt)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState ("up_weapon_hold");
	pAnimState->addTime (dt);
}


void SoldierAnimationComponent::OnLowerBodyIdle (float t, float dt)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState ("leg_stand");
	pAnimState->setEnabled (true);
	pAnimState->setWeight (1.0f);
	pAnimState->setLoop (true);
	pAnimState->addTime (dt);
}


void SoldierAnimationComponent::OnLowerBodyRun (float t, float dt)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState ("leg_run");
	pAnimState->addTime (dt);
}
