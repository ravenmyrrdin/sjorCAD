#ifndef GLFACTORY_H
#define GLFACTORY_H

#include <QColor>
#include <QtOpenGL/QGLWidget>
#include "pole.h"

class Sphere3D;

class OpenGLUtility
{
public:
  static void drawCircle( const Vector3D& center, const Vector3D& normal, float radius );
  static int  sizeCircle();
};

class GLFactory
{
  public:
    virtual ~GLFactory(){}
    virtual void create() const = 0;
    virtual int size() const = 0;
};

class GLPoleFactory : public GLFactory
{
  public:
    GLPoleFactory( const Pole& pole, const QColor& color );
    virtual void create() const;
    virtual int size() const;

  private:
    Pole pole_;
    QColor color_;
};

class GLPlaneFactory : public GLFactory
{
  public:
    GLPlaneFactory( const Vector3D& centerPos, float side, const QColor& color );
    virtual void create() const;
    virtual int size() const;

  private:
    Vector3D centerPos_;
    float side_;
    QColor color_;
};

class GLCylinderFactory : public GLFactory
{
  public:
    GLCylinderFactory( const Vector3D& start, const Vector3D& end, float radius );
    virtual void create() const;
    virtual int size() const;

  private:
    Vector3D start_;
    Vector3D end_;
    float radius_;
};

class GLSphereFactory : public GLFactory
{
  public:
    GLSphereFactory( const Vector3D& center, float radius );
    GLSphereFactory( const Sphere3D& sphere );
    virtual void create() const;
    virtual int size() const;

  private:
    Vector3D center_;
    float radius_;
};

class GLLineFactory : public GLFactory
{
  public:
    GLLineFactory( const Vector3D& begin, const Vector3D& end );
    virtual void create() const;
    virtual int size() const;

  private:
    Vector3D begin_;
    Vector3D end_;
};

#endif