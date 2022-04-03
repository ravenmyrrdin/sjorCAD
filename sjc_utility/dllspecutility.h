#ifndef DLLSPECUTILITY_H
#define DLLSPECUTILITY_H

#ifdef SJCUTILITYDLL
#define DLL_EXPORT_UTILITY __declspec(dllexport)
#else
#define DLL_EXPORT_UTILITY __declspec(dllimport)
#endif

#endif