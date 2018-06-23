#pragma once

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

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
