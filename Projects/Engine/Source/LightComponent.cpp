#include "LightComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "RenderSystem.h"
#include "TransformComponent.h"


namespace Engine {

unsigned int LightComponent::s_instanceCount = 0;


LightComponent::Descriptor::Descriptor ()
	: name (""),
	type (Ogre::Light::LT_POINT),
	diffuseColor (Ogre::ColourValue::ZERO),
	specularColor (Ogre::ColourValue::ZERO),
	intensity (0.0f),
	range (0.0f),
	constantAttenuation (0.0f),
	linearAttenuation (0.0f),
	quadricAttenuation (0.0f),
	innerAngle (0.0f),
	outerAngle (0.0f)
{
}


LightComponent::LightComponent (const Descriptor& desc)
	: LightComponent (desc.name, desc.type)
{
}


LightComponent::LightComponent (const std::string& name, const Ogre::Light::LightTypes& t)
	: Component (name),
	m_pSceneMgr (Game::GetInstance ().GetRenderSystem ()->GetSceneManager ()),
	m_pLight (nullptr),
	m_type (t)
{
}


void LightComponent::PostInit (GameObject* object)
{
	Ogre::String lName (object->GetName () + "_light_" + Ogre::StringConverter::toString (s_instanceCount++));
	m_pLight = m_pSceneMgr->createLight (lName);
	if (m_pLight != nullptr)
		m_pLight->setType (m_type);
}


inline void LightComponent::PostUpdate (float t, float dt)
{
	auto ownerTransform = m_owner->Transform ();

	if (m_type != Ogre::Light::LT_DIRECTIONAL)
		m_pLight->setPosition (ownerTransform->GetPositionInWorldSpace ());
	if (m_type != Ogre::Light::LT_POINT)
		m_pLight->setDirection (ownerTransform->GetForwardVecInWorldSpace ());
}


void LightComponent::Destroy ()
{
	if (m_pSceneMgr != nullptr)
		m_pSceneMgr->destroyLight (m_pLight->getName ());
}


inline const Ogre::ColourValue& LightComponent::GetDiffuseColor () const
{
	return m_pLight->getDiffuseColour (); 
}


inline const Ogre::ColourValue& LightComponent::GetSpecularColor () const
{
	return m_pLight->getSpecularColour (); 
}


inline float LightComponent::GetIntensity () const
{
	return m_pLight->getPowerScale (); 
}


float LightComponent::GetAttenRange () const
{
	if (m_type != Ogre::Light::LT_DIRECTIONAL)
		return m_pLight->getAttenuationRange ();
	else {
		std::cout << "Directional light has no attenuation!\n";
		return 0.0f;
	}
}


float LightComponent::GetConstAtten () const
{
	if (m_type != Ogre::Light::LT_DIRECTIONAL)
		return m_pLight->getAttenuationConstant ();
	else {
		std::cout << "Directional light has no attenuation!\n";
		return 0.0f;
	}
}


float LightComponent::GetLinAtten () const
{
	if (m_type != Ogre::Light::LT_DIRECTIONAL)
		return m_pLight->getAttenuationLinear ();
	else {
		std::cout << "Directional light has no attenuation!\n";
		return 0.0f;
	}
}


float LightComponent::GetQuadAtten () const
{
	if (m_type != Ogre::Light::LT_DIRECTIONAL)
		return m_pLight->getAttenuationQuadric ();
	else {
		std::cout << "Directional light has no attenuation!\n";
		return 0.0f;
	}
}


const Ogre::Vector3& LightComponent::GetPosition () const
{
	if (m_type != Ogre::Light::LT_DIRECTIONAL)
		return m_pLight->getPosition ();
	else {
		std::cout << "Directional light has no position!\n";
		return Ogre::Vector3::ZERO;
	}
}


const Ogre::Vector3& LightComponent::GetDirection () const
{
	if (m_type != Ogre::Light::LT_POINT)
		return m_pLight->getDirection ();
	else {
		std::cout << "Point light has no direction!\n";
		return Ogre::Vector3::ZERO;
	}
}


inline void LightComponent::SetDiffuseColor (const Ogre::ColourValue& diffuseColor)
{
	m_pLight->setDiffuseColour (diffuseColor);
}


inline void LightComponent::SetSpecularColor (const Ogre::ColourValue& specularColor)
{
	m_pLight->setSpecularColour (specularColor);
}


inline void LightComponent::SetIntensity (float intensity)
{
	m_pLight->setPowerScale (intensity);
}


inline void LightComponent::SetAttenuation (float range, float constant, float linear, float quadric)
{
	if (m_type != Ogre::Light::LT_DIRECTIONAL)
		m_pLight->setAttenuation (range, constant, linear, quadric);
}


inline void LightComponent::SetSpotRange (Ogre::Degree innerAngle, Ogre::Degree outerAngle)
{
	if (m_type == Ogre::Light::LT_SPOTLIGHT)
		m_pLight->setSpotlightRange (innerAngle, outerAngle);
}


void LightComponent::ApplyDescriptor (const Descriptor& desc)
{
	SetDiffuseColor (desc.diffuseColor);
	SetSpecularColor (desc.specularColor);
	SetIntensity (desc.intensity);
	SetAttenuation (desc.range, desc.constantAttenuation, desc.linearAttenuation, desc.quadricAttenuation);
	SetSpotRange (Ogre::Degree (desc.innerAngle), Ogre::Degree (desc.outerAngle));
}

}	// namespace Engine
