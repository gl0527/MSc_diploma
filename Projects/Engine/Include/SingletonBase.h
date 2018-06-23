#pragma once

#ifndef SINGLETON_BASE_H
#define SINGLETON_BASE_H

// ---------------------------------- includes ----------------------------------

#include <memory>
#include <mutex>


namespace Engine {

// ============================ class SingletonBase =============================

template <typename T>
class SingletonBase
{
public:
	static void	DeleteInstance ();
	static bool IsExist ();

protected:
	struct Deleter;

	using UPtr = std::unique_ptr<T, Deleter>;

	static UPtr				s_pInstance;
	static std::once_flag	s_onceFlag;

					SingletonBase () = default;
					~SingletonBase () = default;
					SingletonBase (const SingletonBase&) = delete;
	SingletonBase*	operator= (const SingletonBase&) = delete;
};


template<typename T>
typename SingletonBase<T>::UPtr SingletonBase<T>::s_pInstance = nullptr;

template<typename T>
std::once_flag	SingletonBase<T>::s_onceFlag;

template<typename T>
struct SingletonBase<T>::Deleter
{
	void operator () (T* deletable)
	{
		if (deletable != nullptr) {
			delete deletable;
			deletable = nullptr;
		}
	}
};


template<typename T>
inline void SingletonBase<T>::DeleteInstance ()
{
	std::call_once (s_onceFlag, [] () { s_pInstance.reset (); });
}


template<typename T>
inline bool SingletonBase<T>::IsExist ()
{
	return (s_pInstance != nullptr);
}

}	// namespace Engine

#endif	// #ifndef SINGLETON_BASE_H
