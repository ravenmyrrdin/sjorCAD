######################################################################
# Automatically generated by qmake (2.01a) Tue Sep 23 21:23:21 2008
######################################################################

TEMPLATE = lib
DESTDIR = ..\..\bin
TARGET = sjc_opengl
DEPENDPATH += .
INCLUDEPATH += ..\sjc_utility ..\sjc_construction
QT += opengl
LIBS += ..\..\bin\sjc_utility.lib ..\..\bin\sjc_construction.lib
DEFINES += SJCOPENGLDLL

# Input
HEADERS += myglwidget.h dllspecopengl.h globject.h glfactory.h glconstructionview.h glpoledelegate.h
SOURCES += myglwidget.cpp globject.cpp glfactory.cpp glconstructionview.cpp glpoledelegate.cpp
