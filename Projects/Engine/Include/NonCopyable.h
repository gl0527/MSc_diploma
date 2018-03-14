#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

#pragma once

namespace Engine {

// ============================== class NonCopyable ==============================

class NonCopyable
{
public:
					NonCopyable () = default;
					NonCopyable (const NonCopyable&) = delete;
	
	NonCopyable& 	operator= (const NonCopyable&) = delete;
};

}	// namespace Engine

#endif	// #ifndef NONCOPYABLE_H
