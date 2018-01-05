#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

#pragma once

#define WRAP(statements) do { statements } while (0)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define ERR_PREFIX "[ " __TIME__ " ] At " __FILE__ "(" TOSTRING (__LINE__) ")"

#define ERR_LOG(os, msg) os << ERR_PREFIX << ": " << msg
#define ERR_THROW(exceptionType, errMsg) throw exceptionType (ERR_PREFIX + std::string (": ") + errMsg)

#endif	// ERROR_MESSAGE_H
