#ifndef DELEGATE_H
#define DELEGATE_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include <vector>
#include <algorithm>
#include <functional>


namespace Engine {

// =============================== class Delegate ===============================

template<typename RET = void, typename... ARGS>
class Delegate
{
private:
	using Callback = std::function<RET (ARGS...)>;
	std::vector<Callback> m_handlers;

public:
	Delegate () = default;
	
	Delegate (Callback callback)
	{
		m_handlers.push_back (callback);
	}

	Delegate (std::nullptr_t nullPtr)
	{
	}


	void operator= (Callback callback)
	{
		m_handlers.clear ();
		m_handlers.push_back (callback);
	}

	void operator= (std::nullptr_t nullPtr)
	{
		m_handlers.clear ();
	}

	Delegate operator+ (Callback callback)
	{
		Delegate del (*this);
		del += callback;
		return del;
	}

	Delegate operator+ (Delegate other)
	{
		Delegate del (*this);
		del.m_handlers.insert (del.m_handlers.end (), other.m_handlers.begin (), other.m_handlers.end ());
		return del;
	}

	Delegate& operator+ (std::nullptr_t nullPtr)
	{
		return *this;
	}

	void operator+= (Callback callback)
	{
		m_handlers.push_back (callback);
	}

	void operator+= (Delegate other)
	{
		m_handlers.insert (m_handlers.end (), other.m_handlers.begin (), other.m_handlers.end ());
	}

	void operator+= (std::nullptr_t nullPtr)
	{
	}

	Delegate operator- (Callback callback)
	{
		Delegate del (*this);
		del -= callback;
		return del;
	}

	Delegate& operator- (std::nullptr_t nullPtr)
	{
		return *this;
	}

	void operator-= (Callback callback)
	{
		m_handlers.erase (std::remove (m_handlers.begin (), m_handlers.end (), callback), m_handlers.end ());
	}

	void operator-= (std::nullptr_t nullPtr)
	{
	}

	void operator() (ARGS... args)
	{
		for (auto& handler : m_handlers)
			handler (std::forward<ARGS> (args)...);
	}
};

using DefaultDelegate = Delegate<>;

}	// namespace Engine

#endif	// #ifndef DELEGATE_H
