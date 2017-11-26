#ifndef PREFAB_BASE_H
#define PREFAB_BASE_H

#pragma once

namespace Engine {

class GameObject;

// ============================== class PrefabBase ===============================

class PrefabBase
{
public:
	PrefabBase (bool immediateCreation);
	virtual ~PrefabBase () {}

	void Init (GameObject* owner);
	bool IsCreationImmediate () const;

	virtual void ApplyPrimaryParams () {}
	virtual void ApplySecondaryParams () {}

protected:
	GameObject* m_owner;
	bool m_immediateCreation;
};

}	// namespace Engine

#endif	// #ifndef PREFAB_BASE_H
