#ifndef DLLSPECOPENGL_H
#define DLLSPECOPENGL_H

#ifdef SJCOPENGLDLL
#define DLL_EXPORT_OPENGL __declspec(dllexport)
#else
#define DLL_EXPORT_OPENGL __declspec(dllimport)
#endif

#endif