#include "SoldierAnimationComponent.h"
#include "InputManager.h"
#include "MeshComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonInstance.h"
#include "OgreEntity.h"
#include "AudioSourceComponent.h"
#include "PlayerDataComponent.h"
#include "ObjectManager.h"
#include "ManagerComponent.h"


namespace {

inline bool IsRunningBackwards ()
{
	return InputManager::GetInstance ().IsKeyDown (OIS::KC_S);
}

inline bool IsRunning ()
{
	return	InputManager::GetInstance ().IsKeyDown (OIS::KC_W) ||
			InputManager::GetInstance ().IsKeyDown (OIS::KC_S);
}

}	// namespace


SoldierAnimationComponent::SoldierAnimationComponent (const std::string& name):
	AnimationComponent (name),
	m_upperBodyAnimation (UpperBodyState::Idle),
	m_lowerBodyAnimation (LowerBodyState::Idle),
	m_ownerData (nullptr),
	m_isInShootState (false),
	m_hasWeapon (false),
	m_isDead (false)
{
	m_upperBodyAnimation.AddTransitions ({
		{UpperBodyState::Idle,			UpperBodyState::Run,		'r', [this] { Transition ("up_stand", 0.0f, "up_run", 1.0f, true); }},
		{UpperBodyState::Idle,			UpperBodyState::WeaponHold,	'w', [this] { Transition ("up_stand", 0.0f, "up_weapon_hold", 1.0f, true); }},
		{UpperBodyState::Run,			UpperBodyState::Idle,		'i', [this] { Transition ("up_run", 0.0f, "up_stand", 1.0f, true); }},
		{UpperBodyState::Run,			UpperBodyState::WeaponHold,	'w', [this] { Transition ("up_run", 0.0f, "up_weapon_hold", 1.0f, true); }},
		{UpperBodyState::WeaponHold,	UpperBodyState::Shoot,		's', [this] { Transition ("up_weapon_hold", 0.0f, "up_shoot", 1.0f, false); }},
		{UpperBodyState::Shoot,			UpperBodyState::WeaponHold,	'w', [this] { Transition ("up_shoot", 0.0f, "up_weapon_hold", 1.0f, true); }},
	});

	m_lowerBodyAnimation.AddTransitions ({
		{LowerBodyState::Idle,	LowerBodyState::Run,	'r', [this] { Transition ("leg_stand", 0.0f, "leg_run", 1.0f, true); }},
		{LowerBodyState::Run,	LowerBodyState::Idle,	'i', [this] { Transition ("leg_run", 0.0f, "leg_stand", 1.0f, true); }},
		{LowerBodyState::Idle,	LowerBodyState::Dead,	'd', [this] { TransitionToDeath (); }},
		{LowerBodyState::Run,	LowerBodyState::Dead,	'd', [this] { TransitionToDeath (); }},
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

	if (auto managerObj = ObjectManager::GetInstance ().GetGameObjectByName ("manager").lock ()) {
		m_managerComp = managerObj->GetFirstComponentByType<ManagerComponent> ().lock ();
	}

	if (m_managerComp == nullptr) {
		m_owner->RemoveComponent (m_name);
		return;
	}

	if (auto ownerMesh = m_owner->GetFirstComponentByType<MeshComponent> ().lock ()) {
		m_ownerEntity = ownerMesh->GetEntity ();

		if (auto skeleton = m_ownerEntity->getSkeleton ()) {
			skeleton->setBlendMode (Ogre::ANIMBLEND_CUMULATIVE);

			Enable ("up_stand", true);
			Enable ("leg_stand", true);

			return;
		}
	}
	m_owner->RemoveComponent (m_name);
}


void SoldierAnimationComponent::PostUpdate (float t, float dt)
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
	} else {
		m_upperBodyAnimation.Process (IsRunning () ? 'r' : 'i');
	}
	m_lowerBodyAnimation.Process (IsRunning () ? 'r' : 'i');

	m_upperBodyAnimation.Update (t, !m_isInShootState && IsRunningBackwards () ? -dt : dt);
	m_lowerBodyAnimation.Update (t, IsRunningBackwards () ? -dt : dt);
}


void SoldierAnimationComponent::HasWeapon (bool hasWeapon)
{
	m_hasWeapon = hasWeapon;
}


bool SoldierAnimationComponent::IsDead () const
{
	return m_isDead;
}


void SoldierAnimationComponent::TransitionToDeath ()
{
	Disable ("up_stand");
	Disable ("up_run");
	Disable ("up_weapon_hold");
	Disable ("up_shoot");
	Disable ("leg_stand");
	Disable ("leg_run");

	Enable ("death", false);
	m_upperBodyAnimation.SetState (UpperBodyState::Dead);
}


void SoldierAnimationComponent::OnUpperBodyIdle (float t, float dt)
{
	Step ("up_stand", dt);
}


void SoldierAnimationComponent::OnUpperBodyRun (float t, float dt)
{
	Step ("up_run", dt);
}


void SoldierAnimationComponent::OnUpperBodyShoot (float t, float dt)
{
	Step ("up_shoot", dt);
	static unsigned int counter = 0;
	if (fabs (GetTimePositionInSeconds ("up_shoot") - 0.8f) < 0.01f) {
		if (auto weapon = m_owner->GetChild ("weapon")) {
			m_managerComp->CreateBullet (counter++, weapon->Transform ()->GetGlobalPosition (), weapon->Transform ()->GetGlobalFacing ());
			if (auto weaponAudio = weapon->GetFirstComponentByType<AudioSourceComponent> ().lock ())
				weaponAudio->Play ();
		}
	}
	if (HasEnded ("up_shoot"))
		m_isInShootState = false;
}


void SoldierAnimationComponent::OnUpperBodyWeaponHold (float t, float dt)
{
	Step ("up_weapon_hold", dt);
}


void SoldierAnimationComponent::OnLowerBodyIdle (float t, float dt)
{
	Step ("leg_stand", dt);
}


void SoldierAnimationComponent::OnLowerBodyRun (float t, float dt)
{
	Step ("leg_run", dt);
}


void SoldierAnimationComponent::OnDeath (float t, float dt)
{
	Step ("death", dt);
	static bool isDeathSoundPlayed = false;
	auto soldierAudio = m_owner->GetFirstComponentByType<AudioSourceComponent> ().lock ();
	if (!isDeathSoundPlayed && soldierAudio != nullptr) {
		soldierAudio->Play ();
		isDeathSoundPlayed = true;
	}
}
