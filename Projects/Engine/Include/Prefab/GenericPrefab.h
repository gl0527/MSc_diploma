#pragma once

#ifndef GENERIC_PREFAB_H
#define GENERIC_PREFAB_H

// ---------------------------------- includes ----------------------------------

#include "Prefab/IPrefab.h"
#include "GameObject.h"
#include <memory>


namespace Engine {
namespace Prefab {

// ============================ class GenericPrefab ============================

template<typename TYPE, typename DESC>
class GenericPrefab : public IPrefab
{
public:
	void Create () override;
	void Attach (GameObject* owner, bool replaceOld = true) override;
	void ApplyDescriptor () override;

	void SetDescriptor (const DESC& desc);

private:
	DESC					m_desc;
	std::shared_ptr<TYPE>	m_managedObj;
};


// ============================== implementations ==============================

template<typename TYPE, typename DESC>
inline void GenericPrefab<TYPE, DESC>::Create ()
{
	m_managedObj.reset (new TYPE (m_desc));
}


template<typename TYPE, typename DESC>
inline void GenericPrefab<TYPE, DESC>::Attach (GameObject* owner, bool replaceOld /*= true*/)
{
	if (m_managedObj != nullptr)
		owner->AddComponent (m_managedObj, replaceOld);
}

template<typename TYPE, typename DESC>
inline void GenericPrefab<TYPE, DESC>::ApplyDescriptor ()
{
	if (m_managedObj != nullptr)
		m_managedObj->ApplyDescriptor (m_desc);
}


template<typename TYPE, typename DESC>
inline void GenericPrefab<TYPE, DESC>::SetDescriptor (const DESC& desc)
{
	m_desc = desc;
}

}	// namespace Prefab
}	// namespace Engine 

#endif	// #ifndef GENERIC_PREFAB_H
