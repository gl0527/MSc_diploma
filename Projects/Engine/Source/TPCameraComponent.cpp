#include "TPCameraComponent.h"
#include "GameObject.h"
#include "PhysicsSystem.h"
#include "Game.h"
#include "TransformComponent.h"


namespace Engine
{

TPCameraComponent::TPCameraComponent (const std::string& name, int zDepth)
	: CameraComponent (name, zDepth),
	camHeight (0.0f),
	targetHeight (0.0f),
	camDist (0.0f),
	motBlend (0.0f),
	fixed (false),
	physicsSys (Game::GetInstance ().getPhysicsSystem ())
{
}


TPCameraComponent::~TPCameraComponent ()
{}


void TPCameraComponent::Init (GameObject* object)
{
	CameraComponent::Init (object);
	auto& dir = m_owner->Transform ()->forward ();
	dir.normalise ();
	const auto& ownerPos = m_owner->Transform ()->worldPosition ();

	camera->setPosition (ownerPos - dir*camDist + Ogre::Vector3 (0, camHeight, 0));
	camera->lookAt (ownerPos + Ogre::Vector3 (0, targetHeight, 0));
}


void TPCameraComponent::PostUpdate (float t, float dt)
{
	const auto& ownerPos = m_owner->Transform ()->worldPosition () + Ogre::Vector3 (0, targetHeight, 0);
	auto& dir = m_owner->Transform ()->forward ();
	dir.normalise ();
	auto newPos = ownerPos - dir*camDist + Ogre::Vector3 (0, camHeight, 0);

	//int sgn = camera->getDirection().z < 0.0f ? -1 : 1;

	btVector3 btCamPos (camera->getPosition ().x, camera->getPosition ().y, camera->getPosition ().z);
	btVector3 btOwnerPos (ownerPos.x, ownerPos.y, ownerPos.z);
	auto& ray = physicsSys->rayTest (btOwnerPos, btCamPos);

	if (ray.hasHit ()) {
		//auto& hitPoint = ray.m_hitPointWorld;
		//auto& hitNormal = ray.m_hitNormalWorld;
		//if (hitPoint.z() > btOwnerPos.z())
		camDist *= 0.8f;
	}
	else if (fabs (camDist) < 40.0f) {
		camDist *= 1.2f;
	}

	float w = motBlend * dt;

	if (!fixed)
		camera->setPosition (w * newPos + (1.0f - w) * camera->getPosition ());
	camera->lookAt (ownerPos/* + Ogre::Vector3(0, targetHeight, 0)*/);
}

}	// namespace Engine
