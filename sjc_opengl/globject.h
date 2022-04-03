#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QtOpenGL/QGLWidget>

class MyGLWidget;

class GLObject
{
  public:
    GLObject( GLuint begin, int range, const MyGLWidget* glwidget );
    GLuint              begin() const;
    int                 range() const;
    //const MyGLWidget*   glwidget() const;


  private:
    GLuint begin_;
    int range_;
    const MyGLWidget* glwidget_;
};


#endif