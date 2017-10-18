#pragma once

#define MYNAMESPACE_EXPORT

#ifdef MYNAMESPACE_EXPORT
#define DLL_SPEC __declspec(dllexport)
#else
#define DLL_SPEC __declspec(dllimport)
#endif