#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#pragma once

#include "Component.h"
#include "MyGUI_WidgetDefines.h"

using namespace Engine;

namespace MyGUI {

class Button;

}	// namespace MyGUI


class GUIComponent : public Component
{
public:
	explicit GUIComponent (const std::string& name);

	void Start () override;
	void PostUpdate (float t, float dt) override;

private:
	MyGUI::VectorWidgetPtr	m_menu;
	MyGUI::Button*			m_pQuitButton;
	MyGUI::Button*			m_pContinueButton;
};



#endif	// #ifndef GUI_COMPONENT_H
