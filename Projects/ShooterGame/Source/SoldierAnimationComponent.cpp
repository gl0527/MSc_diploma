#include "SoldierAnimationComponent.h"
#include "InputManager.h"
#include "MeshComponent.h"
#include "GameObject.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonInstance.h"
#include "OgreEntity.h"
#include "AudioSourceComponent.h"
#include "PlayerDataComponent.h"


namespace {

inline bool IsRunning ()
{
	return InputManager::GetInstance ().IsKeyDown (OIS::KC_W);
}

}	// namespace


SoldierAnimationComponent::SoldierAnimationComponent (const std::string& name):
	Component (name),
	m_upperBodyAnimation (UpperBodyState::Idle),
	m_lowerBodyAnimation (LowerBodyState::Idle),
	m_ownerEntity (nullptr),
	m_ownerData (nullptr),
	m_isInShootState (false),
	m_hasWeapon (false),
	m_isDead (false)
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
		{LowerBodyState::Idle,	LowerBodyState::Dead,	'd', [this] { OnTransitionToDeath (); }},
		{LowerBodyState::Run,	LowerBodyState::Dead,	'd', [this] { OnTransitionToDeath (); }},
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

	m_lowerBodyAnimation.AddStateFunction (LowerBodyState::Dead,
		std::bind (&SoldierAnimationComponent::OnDeath, this, std::placeholders::_1, std::placeholders::_2));
}


void SoldierAnimationComponent::Start ()
{
	m_ownerData = m_owner->GetFirstComponentByType<PlayerDataComponent> ().lock ();
	if (m_ownerData == nullptr) {
		m_owner->RemoveComponent (m_name);
		return;
	}
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
	if (m_ownerData->IsDead ()) {
		m_isDead = true;
		m_lowerBodyAnimation.Process ('d');
	}
	if (m_hasWeapon) {
		if ((m_isInShootState || InputManager::GetInstance ().IsLeftMouseButtonDown ())) {
			m_isInShootState = true;
			m_upperBodyAnimation.Process ('s');
		} else {
			m_upperBodyAnimation.Process ('w');
		}
		m_lowerBodyAnimation.Process (IsRunning () ? 'r' : 'i');
	} else {
		m_upperBodyAnimation.Process (IsRunning () ? 'r' : 'i');
		m_lowerBodyAnimation.Process (IsRunning () ? 'r' : 'i');
	}
	m_upperBodyAnimation.Update (t, dt);
	m_lowerBodyAnimation.Update (t, dt);
}


void SoldierAnimationComponent::HasWeapon (bool hasWeapon)
{
	m_hasWeapon = hasWeapon;
}


bool SoldierAnimationComponent::IsDead () const
{
	return m_isDead;
}


void SoldierAnimationComponent::OnTransition (const char* fromAnimName, const char* toAnimName, bool isLooping)
{
	TurnOffAnimation (fromAnimName);
	TurnOnAnimation (toAnimName, isLooping);
}


void SoldierAnimationComponent::OnTransitionToDeath ()
{
	TurnOffAnimation ("up_stand");
	TurnOffAnimation ("up_run");
	TurnOffAnimation ("up_weapon_hold");
	TurnOffAnimation ("up_shoot");
	TurnOffAnimation ("leg_stand");
	TurnOffAnimation ("leg_run");

	TurnOnAnimation ("death", false);
	m_upperBodyAnimation.SetState (UpperBodyState::Dead);
}


void SoldierAnimationComponent::TurnOffAnimation (const char* animName)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState (animName);
	pAnimState->setEnabled (false);
	pAnimState->setWeight (0.0f);
	pAnimState->setLoop (false);
}


void SoldierAnimationComponent::TurnOnAnimation (const char* animName, bool isLooping)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState (animName);
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

	if (fabs (pAnimState->getTimePosition () - 1.0f) < 0.01f) {
		if (auto weapon = m_owner->GetChild ("weapon")) {
			if (auto weaponAudio = weapon->GetFirstComponentByType<AudioSourceComponent> ().lock ())
				weaponAudio->Play ();
		}
	}
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


void SoldierAnimationComponent::OnDeath (float t, float dt)
{
	Ogre::AnimationState* pAnimState = m_ownerEntity->getAnimationState ("death");
	pAnimState->addTime (dt);
	static bool isDeathSoundPlayed = false;
	auto soldierAudio = m_owner->GetFirstComponentByType<AudioSourceComponent> ().lock ();
	
	if (!isDeathSoundPlayed && soldierAudio != nullptr) {
		soldierAudio->Play ();
		isDeathSoundPlayed = true;
	}
}