#include "AnimationComponent.h"
#include "GameObject.h"
#include "MeshComponent.h"

namespace Engine
{

AnimationComponent::AnimationComponent (const std::string& name, const Ogre::SkeletonAnimationBlendMode& blendMode)
	:Component (name),
	hasSkeleton (false),
	entity (nullptr),
	anim (nullptr),
	animBlend (blendMode)
{
}


AnimationComponent::~AnimationComponent ()
{
}


void AnimationComponent::Start ()
{
	if (auto& ownerMesh = m_owner->getFirstComponentByType<MeshComponent> ().lock ()) {
		entity = ownerMesh->getEntity ();
		if (entity)
			if (auto skeleton = entity->getSkeleton ()) {
				skeleton->setBlendMode (animBlend);
				hasSkeleton = true;
			}
		//else
			//ownerObject->removeComponent(name);
	}
	else
		m_owner->removeComponent (m_name);
}


void AnimationComponent::stop ()
{
	if (hasSkeleton) {
		if (anim)
			anim->setEnabled (false);
	}
}


void AnimationComponent::init (const std::string& animName)
{
	if (hasSkeleton)
		anim = entity->getAnimationState (animName);
}


void AnimationComponent::start ()
{
	if (hasSkeleton) {
		if (anim)
			anim->setEnabled (true);
	}
}

void AnimationComponent::setEnabled (bool enable)
{
	if (hasSkeleton && anim)
		anim->setEnabled (enable);
}


void AnimationComponent::setWeight (unsigned int weight)
{
	if (hasSkeleton) {
		if (anim)
			anim->setWeight (weight);
	}
}


void AnimationComponent::setLoop (bool loop)
{
	if (hasSkeleton) {
		if (anim)
			anim->setLoop (loop);
	}
}


bool AnimationComponent::getLoop ()
{
	if (hasSkeleton) {
		if (anim)
			return anim->getLoop ();
	}
	return false;
}


void AnimationComponent::setTime (float time)
{
	if (hasSkeleton) {
		if (anim)
			anim->setTimePosition (time);
	}
}


void AnimationComponent::step (float time)
{
	if (hasSkeleton) {
		if (anim)
			anim->addTime (time);
	}
}


bool AnimationComponent::ended ()
{
	if (hasSkeleton) {
		if (anim)
			return anim->hasEnded ();
	}
	return false;
}

}	// namespace Engine
