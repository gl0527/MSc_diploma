#ifndef BULLET_COMPONENT_H
#define BULLET_COMPONENT_H

#pragma once


#include "Component.h"

namespace Engine {

class PhysicsComponent;

}	// namespace Engine


using namespace Engine;



class BulletComponent : public Component
{
public:
	explicit BulletComponent (const std::string& name);

	void Start () override;


private:
	void OnCollisionWithEnemy (PhysicsComponent* other);

};



#endif	// #ifndef BULLET_COMPONENT_H
