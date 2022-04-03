#include "globject.h"

GLObject::GLObject( GLuint begin, int range, const MyGLWidget* glwidget )
: begin_( begin ), range_( range ), glwidget_( glwidget )
{
}

GLuint GLObject::begin() const
{
  return begin_;
}

int GLObject::range() const
{
  return range_;
}

/*const MyGLWidget* GLObject::glwidget() const
{
  return glwidget_;
}*/
