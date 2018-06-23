#pragma once

#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

// ---------------------------------- includes ----------------------------------

#include "Component.h"


namespace Ogre {

class MovableObject;
class SceneManager;
class SceneNode;

}

namespace Engine {

// ============================ class RenderComponent ============================

class RenderComponent : public Component
{
public:
	explicit			RenderComponent (const std::string& name);
	virtual				~RenderComponent ();

	void				PostInit (GameObject* obj) override;
	void				PostUpdate (float t, float dt) override;
	void				Destroy () override;

	Ogre::SceneNode*	GetOgreNode () const { return m_pCurrentNode; }

	void				SetCastShadows (bool cast);
	void				SetVisible (bool visible);

	DLL_EXPORT void		MoveNode ();

protected:
	Ogre::SceneNode*		m_pParentNode;
	Ogre::SceneNode*		m_pCurrentNode;
	Ogre::MovableObject*	m_pObject;
	Ogre::SceneManager*		m_pSceneManager;


	void				CreateNode ();
};

}	// namespace Engine

#endif	// #ifndef RENDER_COMPONENT_H