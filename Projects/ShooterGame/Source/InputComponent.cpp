#include "InputComponent.h"
#include "GameObject.h"
#include "InputHandler.h"
#include <Ogre.h>
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

	/*if (inputHandler->isKeyDown(OIS::KC_ESCAPE))
	{
		Game::getInstance().destroy();
		return;
	}*/
	/*if (inputHandler->isKeyDown(OIS::KC_P))
		Game::getInstance().pause();*/
	/*if (inputHandler->isKeyDown(OIS::KC_D))
		moveDir += Ogre::Vector3(1.0f, 0.0f, 0.0f);
	if (inputHandler->isKeyDown(OIS::KC_A))
		moveDir += Ogre::Vector3(-1.0f, 0.0f, 0.0f);
	if (inputHandler->isKeyDown(OIS::KC_W))
		moveDir += Ogre::Vector3(0.0f, 0.0f, -1.0f);
	if (inputHandler->isKeyDown(OIS::KC_S))
		moveDir += Ogre::Vector3(0.0f, 0.0f, 1.0f);
	if (inputHandler->isKeyDown(OIS::KC_SYSRQ) && delay > maxDelay)
	{
		std::cout << "Screenshot taken.\n";
		Game::getInstance().getRenderSystem()->getRenderWindow()->writeContentsToTimestampedFile("screenshot", ".jpg");
		delay = 0;
	}
		
	if (ms.buttonDown(OIS::MB_Left))
		std::cout << "lmb: " << ms.X.abs << ", " << ms.Y.abs << std::endl;
	if (ms.buttonDown(OIS::MB_Right))
		std::cout << "rmb: " << ms.X.rel << ", " << ms.Y.rel << std::endl;*/
	
	Ogre::Radian yaw(-turnSpeed * InputHandler::GetInstance ().GetRelativeMouseX ());
	Ogre::Quaternion Qyaw(yaw, Ogre::Vector3::UNIT_Y);
	m_owner->Transform()->setRotation(Qyaw * m_owner->Transform()->worldRotation());

	const Ogre::Vector3& right = m_owner->Transform()->right();
	Ogre::Radian pitch(-turnSpeed * InputHandler::GetInstance ().GetRelativeMouseY ());
	Ogre::Quaternion Qpitch(pitch, right);
	m_owner->Transform()->setRotation(Qpitch * m_owner->Transform()->worldRotation());

	/*moveDir.normalise();
	moveDir = m_owner->transform()->rotation() * moveDir; // azert, hogy a movedir az m_owner koordinata-rendszereben legyen ertve
	
	m_owner->transform()->setPosition(m_owner->transform()->position() + moveDir * dt * moveSpeed);*/ // a dt miatt a mozgas sebessege fuggetlen a gep sebessegetol
}


InputComponent::~InputComponent()
{
}
