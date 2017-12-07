#include "TransformComponent.h"
#include "GameObject.h"


namespace Engine {

void TransformComponent::Start ()
{
	if (auto parent = m_owner->GetParent ().lock ()) {
		auto parentTrans = parent->Transform ();
		auto parentPos = parentTrans->worldPosition ();
		localPos = globalPos - parentPos;
	}
	else {
		localPos = globalPos;
		localRot = globalRot;
		localScale = globalScale;
	}
}

}	// namespace Engine
