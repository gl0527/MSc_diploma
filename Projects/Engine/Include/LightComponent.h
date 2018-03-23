#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"
#include "OgreLight.h"


namespace Engine {

// ============================ class LightComponent ============================

class LightComponent : public Component
{
public:
	struct Descriptor
	{
		Descriptor ();

		std::string name;
		Ogre::Light::LightTypes type;

		Ogre::ColourValue diffuseColor;
		Ogre::ColourValue specularColor;
		float intensity;
		float range;
		float constantAttenuation;
		float linearAttenuation;
		float quadricAttenuation;
		float innerAngle;
		float outerAngle;
	};

								LightComponent (const Descriptor& desc);
								LightComponent (const std::string& name, const Ogre::Light::LightTypes& t);

	virtual void				PostInit (GameObject* object) override;
	virtual void				PostUpdate (float t, float dt) override;
	virtual void				Destroy () override;

	const Ogre::ColourValue&	GetDiffuseColor () const;
	const Ogre::ColourValue&	GetSpecularColor () const;
	float						GetIntensity () const;
	float						GetAttenRange () const;
	float						GetConstAtten () const;
	float						GetLinAtten () const;
	float						GetQuadAtten () const;
	const Ogre::Vector3&		GetPosition () const;
	const Ogre::Vector3&		GetDirection () const;

	void						SetDiffuseColor (const Ogre::ColourValue& diffuseColor);
	void						SetSpecularColor (const Ogre::ColourValue& specularColor);
	void						SetIntensity (float intensity);
	void						SetAttenuation (float range, float constant, float linear, float quadric);
	void						SetSpotRange (Ogre::Degree innerAngle, Ogre::Degree outerAngle);

	void						ApplyDescriptor (const Descriptor& desc);

private:
	static unsigned int		s_instanceCount;

	Ogre::SceneManager*		m_pSceneMgr;
	Ogre::Light*			m_pLight;
	Ogre::Light::LightTypes m_type;
};

}	// namespace Engine

#endif	// #ifndef LIGHT_COMPONENT_H
