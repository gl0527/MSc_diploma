#include "InputComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreMath.h"
#include "TransformComponent.h"


InputComponent::InputComponent(const std::string& ident) :
	Component(ident),
	moveSpeed(50.0f),
	turnSpeed(0.001f),
	mouseSensitivity(50.0f),
	delay(0),
	maxDelay(20)
{
}


void InputComponent::PreUpdate(float t, float dt)
{	
	Ogre::Vector3 moveDir = Ogre::Vector3::ZERO;
	++delay;

	if (InputManager::GetInstance ().IsKeyDown (OIS::KC_D))
		moveDir += Ogre::Vector3(-1.0f, 0.0f, 0.0f);
	if (InputManager::GetInstance ().IsKeyDown (OIS::KC_A))
		moveDir += Ogre::Vector3(1.0f, 0.0f, 0.0f);
	if (InputManager::GetInstance ().IsKeyDown (OIS::KC_W))
		moveDir += Ogre::Vector3(0.0f, 0.0f, 1.0f);
	if (InputManager::GetInstance ().IsKeyDown (OIS::KC_S))
		moveDir += Ogre::Vector3(0.0f, 0.0f, -1.0f);
	/*if (inputHandler->isKeyDown(OIS::KC_SYSRQ) && delay > maxDelay)
	{
		std::cout << "Screenshot taken.\n";
		Game::getInstance().getRenderSystem()->getRenderWindow()->writeContentsToTimestampedFile("screenshot", ".jpg");
		delay = 0;
	}*/
		
	int mouseRelX, mouseRelY;

	if (InputManager::GetInstance ().GetRelativeMouseX (&mouseRelX)) {
		Ogre::Radian yaw (-turnSpeed * mouseRelX);
		Ogre::Quaternion Qyaw (yaw, Ogre::Vector3::UNIT_Y);
		m_owner->Transform ()->SetGlobalRotation (Qyaw * m_owner->Transform ()->GetGlobalRotation ());
	}

	/*const Ogre::Vector3& right = m_owner->Transform()->Right();
	
	if (InputManager::GetInstance ().GetRelativeMouseY (&mouseRelY)) {
		Ogre::Radian pitch (-turnSpeed * mouseRelY);
		Ogre::Quaternion Qpitch (pitch, right);
		m_owner->Transform ()->SetGlobalRotation (Qpitch * m_owner->Transform ()->GetGlobalRotation ());
	}*/

	moveDir.normalise ();
	moveDir = m_owner->Transform ()->GetGlobalRotation () * moveDir;
	m_owner->Transform ()->SetGlobalPosition (m_owner->Transform ()->GetGlobalPosition () + moveDir * dt * moveSpeed);
}


InputComponent::~InputComponent()
{
}
