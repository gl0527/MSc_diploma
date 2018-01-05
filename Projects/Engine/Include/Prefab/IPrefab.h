#ifndef IPREFAB_H
#define IPREFAB_H

#pragma once


namespace Engine {

// ----------------------------- forward declaration -----------------------------

class GameObject;

namespace Prefab {

// ================================ class IPrefab ================================

class IPrefab {
public:
	virtual void Create () = 0;
	virtual void Attach (GameObject* owner, bool replaceOld = true) = 0;
	virtual void ApplyDescriptor () = 0;
};

}	// namespace Prefab
}	// namespace Engine 

#endif	// #ifndef IPREFAB_H
