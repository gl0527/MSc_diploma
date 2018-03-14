#ifndef STDAFX_H
#define STDAFX_H

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define WINDOWS
#endif

#define DLL_EXPORT __declspec(dllexport)

#endif	// #ifndef STDAFX_H