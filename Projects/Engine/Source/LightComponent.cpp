#include "LightComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "RenderSystem.h"
#include "TransformComponent.h"


namespace Engine {

unsigned int LightComponent::instanceCount = 0;


LightComponent::LightComponent (const Descriptor& desc)
	: LightComponent (desc.name, desc.type)
{
}


LightComponent::LightComponent (const std::string& name, const Ogre::Light::LightTypes& t)
	: Component (name),
	sceneMgr (Game::GetInstance ().GetRenderSystem ()->getSceneManager ()),
	light (nullptr),
	type (t)
{
}


void LightComponent::PostInit (GameObject* object)
{
	Ogre::String lName (object->GetName () + "_light_" + Ogre::StringConverter::toString (instanceCount++));
	light = sceneMgr->createLight (lName);
	if (light != nullptr)
		light->setType (type);
}


inline void LightComponent::PostUpdate (float t, float dt)
{
	auto ownerTransform = m_owner->Transform ();

	if (type != Ogre::Light::LT_DIRECTIONAL)
		light->setPosition (ownerTransform->worldPosition ());
	if (type != Ogre::Light::LT_POINT)
		light->setDirection (ownerTransform->forward ());
}


void LightComponent::Destroy ()
{
	if (sceneMgr != nullptr)
		sceneMgr->destroyLight (light->getName ());
}


float LightComponent::getAttenuationRange () const
{
	if (type != Ogre::Light::LT_DIRECTIONAL)
		return light->getAttenuationRange ();
	else {
		std::cout << "Directional light has no attenuation!\n";
		return 0.0f;
	}
}


float LightComponent::getConstantAttenuation () const
{
	if (type != Ogre::Light::LT_DIRECTIONAL)
		return light->getAttenuationConstant ();
	else {
		std::cout << "Directional light has no attenuation!\n";
		return 0.0f;
	}
}


float LightComponent::getLinearAttenuation () const
{
	if (type != Ogre::Light::LT_DIRECTIONAL)
		return light->getAttenuationLinear ();
	else {
		std::cout << "Directional light has no attenuation!\n";
		return 0.0f;
	}
}


float LightComponent::getQuadricAttenuation () const
{
	if (type != Ogre::Light::LT_DIRECTIONAL)
		return light->getAttenuationQuadric ();
	else {
		std::cout << "Directional light has no attenuation!\n";
		return 0.0f;
	}
}


const Ogre::Vector3& LightComponent::getPosition () const
{
	if (type != Ogre::Light::LT_DIRECTIONAL)
		return light->getPosition ();
	else {
		std::cout << "Directional light has no position!\n";
		return Ogre::Vector3::ZERO;
	}
}


const Ogre::Vector3& LightComponent::getDirection () const
{
	if (type != Ogre::Light::LT_POINT)
		return light->getDirection ();
	else {
		std::cout << "Point light has no direction!\n";
		return Ogre::Vector3::ZERO;
	}
}


void LightComponent::setAttenuation (float range, float constant, float linear, float quadric)
{
	if (type != Ogre::Light::LT_DIRECTIONAL)
		light->setAttenuation (range, constant, linear, quadric);
}


void LightComponent::setSpotRange (Ogre::Degree innerAngle, Ogre::Degree outerAngle)
{
	if (type == Ogre::Light::LT_SPOTLIGHT)
		light->setSpotlightRange (innerAngle, outerAngle);
}


void LightComponent::ApplyDescriptor (const Descriptor& desc)
{
	setDiffuseColor (desc.diffuseColor);
	setSpecularColor (desc.specularColor);
	setIntensity (desc.intensity);
	setAttenuation (desc.range, desc.constantAttenuation, desc.linearAttenuation, desc.quadricAttenuation);
	setSpotRange (Ogre::Degree (desc.innerAngle), Ogre::Degree (desc.outerAngle));
}

}	// namespace Engine
