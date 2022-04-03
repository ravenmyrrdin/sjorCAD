#ifndef DLLSPECCONSTRUCION_H
#define DLLSPECCONSTRUCION_H

#ifdef SJCCONSTRUCTIONDLL
#define DLL_EXPORT_CONSTRUCTION __declspec(dllexport)
#else
#define DLL_EXPORT_CONSTRUCTION __declspec(dllimport)
#endif

#endif