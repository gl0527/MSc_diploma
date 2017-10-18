#include "SoldierComponent.h"
#include "GameObject.h"

SoldierComponent::SoldierComponent(const std::string& name)
	: Component(name),
	action(PlayerAction::PA_NONE),
	lastAction(action),
	animComp(std::shared_ptr<AnimationComponent>(nullptr))
{
}


SoldierComponent::~SoldierComponent()
{
}


void SoldierComponent::Start()
{
	animComp = m_owner->getFirstComponentByType<AnimationComponent>();
	if(animComp.lock() == nullptr)
		m_owner->removeComponent(m_name);
}


void SoldierComponent::PostUpdate(float t, float dt)
{
	if (auto& anim = animComp.lock())
	{
		anim->init("leg_stand");
		anim->setWeight(1);
		anim->setLoop(action == PlayerAction::PA_NONE);
		anim->setEnabled(action == PlayerAction::PA_NONE);
		anim->step(dt);

		anim->init("up_stand");
		anim->setWeight(1);
		anim->setLoop(action == PlayerAction::PA_NONE);
		anim->setEnabled(action == PlayerAction::PA_NONE);
		anim->step(dt);

		anim->init("leg_run");
		anim->setWeight(1);
		anim->setLoop(true);
		anim->setEnabled(action == PlayerAction::PA_RUN);
		anim->step(dt);

		anim->init("up_run");
		anim->setWeight(1);
		anim->setLoop(true);
		anim->setEnabled(action == PlayerAction::PA_RUN);
		anim->step(dt);

		anim->init("up_shoot");
		anim->setWeight(1);
		anim->setLoop(false);
		anim->setEnabled(action == PlayerAction::PA_SHOOT);
		if (action != lastAction)
			anim->setTime(0.0f);
		anim->step(dt);
		if (anim->ended() && action == PlayerAction::PA_SHOOT) {
			anim->setTime(0.0f);
			action = PlayerAction::PA_NONE;
		}

		lastAction = action;
	}
}