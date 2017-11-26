#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "Component.h"
#include <Ogre.h>


namespace Engine {

// ============================ class LightComponent ============================

class LightComponent : public Component
{
private:
	using Tuple = std::tuple<Ogre::ColourValue, Ogre::ColourValue, float, float, float, float, float, float, float>;	// nehezkes a hasznalata, helyette struct

public:
	struct InitData : public Component::InitData
	{
		InitData ()
			: diffuseColor (Ogre::ColourValue::ZERO),
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

	LightComponent (const std::string& name, const Ogre::Light::LightTypes& t);
	LightComponent (std::tuple<std::string, Ogre::Light::LightTypes> tup);
	~LightComponent () {}

	virtual void PostInit (GameObject* object) override;
	virtual void PostUpdate (float t, float dt) override;
	virtual void Destroy () override;

	const Ogre::ColourValue&	getDiffuseColor () const { return light->getDiffuseColour (); }
	const Ogre::ColourValue&	getSpecularColor () const { return light->getSpecularColour (); }
	float						getIntensity () const { return light->getPowerScale (); }
	float						getAttenuationRange () const;
	float						getConstantAttenuation () const;
	float						getLinearAttenuation () const;
	float						getQuadricAttenuation () const;
	const Ogre::Vector3&		getPosition () const;
	const Ogre::Vector3&		getDirection () const;

	void setDiffuseColor (const Ogre::ColourValue& diffuseColor) { light->setDiffuseColour (diffuseColor); }
	void setSpecularColor (const Ogre::ColourValue& specularColor) { light->setSpecularColour (specularColor); }
	void setIntensity (float intensity) { light->setPowerScale (intensity); }
	void setAttenuation (float range, float constant, float linear, float quadric);
	void setSpotRange (Ogre::Degree innerAngle, Ogre::Degree outerAngle);

	Tuple& GetAttributes ();
	void ApplyAttributes ();
	void ApplyCreationData (const InitData& creationData);

private:
	static unsigned int instanceCount;

	Ogre::SceneManager*		sceneMgr;
	Ogre::Light*			light;
	Ogre::Light::LightTypes type;

	Tuple m_attributes;
};

}	// namespace Engine

#endif	// #ifndef LIGHT_COMPONENT_H
