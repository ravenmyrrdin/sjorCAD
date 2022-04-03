#ifndef DLLSPEC_H
#define DLLSPEC_H

#ifdef SJCTESTUTILITYDLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#endif