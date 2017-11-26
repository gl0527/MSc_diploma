#ifndef PREFAB_H
#define PREFAB_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include "stdafx.h"

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>

#include "GameObject.h"
#include "PrefabBase.h"


namespace Engine {

// ================================ class Prefab =================================

template <typename T>
class Prefab : public PrefabBase
{
public:
	using SPtr = typename std::shared_ptr<T>;

	using PrefabBase::PrefabBase;

	template<typename... Args>
	void StorePrimaryParams (Args&&... args);

	template<size_t Index, typename _Ty>
	void StoreSecondaryParam (_Ty&& value);

	void ApplyPrimaryParams ();
	void ApplySecondaryParams ();

private:
	SPtr								m_pResource;
	std::function<void ()>				m_primaryParamSetter;
	std::vector<std::function<void ()>> m_secondaryParamSetters;
};


template<typename T>
template<typename... Args>
void Prefab<T>::StorePrimaryParams (Args&&... args)
{
	m_primaryParamSetter = [this, tup = std::tuple<Args...> (std::forward<Args> (args)...)] () {
		m_pResource.reset (new T (tup));
	};
}


template<typename T>
template<size_t Index, typename _Ty>
void Prefab<T>::StoreSecondaryParam (_Ty&& value)
{
	auto func = [this, value] () {
		if (m_pResource != nullptr)
			std::get<Index> (m_pResource->GetAttributes ()) = value;
	};

	m_secondaryParamSetters.push_back (func);
}


template<typename T>
void Prefab<T>::ApplyPrimaryParams ()
{
	if (m_primaryParamSetter == nullptr)
		return;

	m_primaryParamSetter ();

	if (m_owner != nullptr)
		m_owner->AddComponent (m_pResource);
}


template<typename T>
void Prefab<T>::ApplySecondaryParams ()
{
	for (auto it = m_secondaryParamSetters.begin (); it != m_secondaryParamSetters.end (); ++it) {
		(*it) ();
	}
	if (m_pResource != nullptr)
		m_pResource->ApplyAttributes ();
}

}	// namespace Engine

#endif	// #ifndef PREFAB_H
