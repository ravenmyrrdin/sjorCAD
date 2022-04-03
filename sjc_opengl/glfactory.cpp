#include "glfactory.h"
#include <QtTest/QtTest>
#include <cmath>

#include "sphere3d.h"

#define M_PI 3.1415

void OpenGLUtility::drawCircle( const Vector3D& center, const Vector3D& normal, float radius )
{
  glPushMatrix();
  glLoadIdentity();
  glTranslatef( center.x(), center.y(), center.z() );
  glRotatef( normal.theta()*180/M_PI, 0, 0, 1 );
  glRotatef( normal.phi()  *180/M_PI, 0, 1, 0 );
  glBegin( GL_LINE_LOOP );
  for( int i = 0; i < 360; ++i )
    glVertex2f( std::cos( i/180.0*M_PI )*radius, std::sin( i/180.0*M_PI )*radius );
  glEnd();
  glPopMatrix();
}

int OpenGLUtility::sizeCircle()
{
  return 368;
}

GLPoleFactory::GLPoleFactory( const Pole& pole, const QColor& color )
: pole_( pole ), color_( color )
{
}

void GLPoleFactory::create() const
{
  glPushMatrix();
  glColor4f(color_.red()/255.0, color_.green()/255.0, color_.blue()/255.0, color_.alpha()/255.0);
  glTranslatef( pole_.center().x(), pole_.center().y(), pole_.center().z() );
  glRotatef( pole_.direction().theta()*180/M_PI, 0, 0, 1 );
  glRotatef( pole_.direction().phi()  *180/M_PI, 0, 1, 0 );
  glTranslatef( 0, 0, -pole_.length()/2 );
  gluCylinder( gluNewQuadric(), pole_.radius(), pole_.radius(), pole_.length(), pole_.radius() * 360, 1 );
  glPopMatrix();
}

int GLPoleFactory::size() const
{
  return 8;
}

GLPlaneFactory::GLPlaneFactory( const Vector3D& centerPos, float side, const QColor& color )
: centerPos_( centerPos ), side_( side ), color_( color )
{
}

void GLPlaneFactory::create() const
{
  glPushMatrix();
  glColor4f(color_.red()/255.0, color_.green()/255.0, color_.blue()/255.0, color_.alpha()/255.0);
  glBegin( GL_LINES );
  glVertex3f( centerPos_.x() - side_/2, centerPos_.y() - side_/2, centerPos_.z() );
  glVertex3f( centerPos_.x() - side_/2, centerPos_.y() + side_/2, centerPos_.z() );

  glVertex3f( centerPos_.x() - side_/2, centerPos_.y() + side_/2, centerPos_.z() );
  glVertex3f( centerPos_.x() + side_/2, centerPos_.y() + side_/2, centerPos_.z() );

  glVertex3f( centerPos_.x() + side_/2, centerPos_.y() + side_/2, centerPos_.z() );
  glVertex3f( centerPos_.x() + side_/2, centerPos_.y() - side_/2, centerPos_.z() );

  glVertex3f( centerPos_.x() + side_/2, centerPos_.y() - side_/2, centerPos_.z() );
  glVertex3f( centerPos_.x() - side_/2, centerPos_.y() - side_/2, centerPos_.z() );
  glEnd();
  glPopMatrix();
}

int GLPlaneFactory::size() const
{
  return 13;
}

GLCylinderFactory::GLCylinderFactory( const Vector3D& start, const Vector3D& end, float radius )
: start_( start ), end_( end ), radius_( radius )
{
}

void GLCylinderFactory::create() const
{
  glPushMatrix();
  glLoadIdentity();
  QColor color( Qt::white );
  glColor4f(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
  Vector3D up = end_ - start_;
  OpenGLUtility::drawCircle( start_, qFuzzyCompare( up.length(), 0.0f ) ? Vector3D( 0,0,1, Vector3D::VECTOR3D ) : up, radius_ );
  OpenGLUtility::drawCircle( end_,   qFuzzyCompare( up.length(), 0.0f ) ? Vector3D( 0,0,1, Vector3D::VECTOR3D ) : up, radius_ );
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  glTranslatef( start_.x(), start_.y(), start_.z() );
  Vector3D direction = end_ - start_;
  glRotatef( direction.theta()*180/M_PI, 0, 0, 1 );
  glRotatef( direction.phi()  *180/M_PI, 0, 1, 0 );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glPopMatrix();
}

int GLCylinderFactory::size() const
{
  return 10 + 2*OpenGLUtility::sizeCircle();
}

GLSphereFactory::GLSphereFactory( const Vector3D& center, float radius )
: center_( center ), radius_( radius )
{
}

GLSphereFactory::GLSphereFactory( const Sphere3D& sphere )
: center_( sphere.center() ), radius_( sphere.radius() )
{
}

void GLSphereFactory::create() const
{
  QColor color( Qt::white );
  glColor4f(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
  OpenGLUtility::drawCircle( center_, Vector3D(1,0,0,Vector3D::POINT3DCART), radius_ );
  OpenGLUtility::drawCircle( center_, Vector3D(0,1,0,Vector3D::POINT3DCART), radius_ );
  OpenGLUtility::drawCircle( center_, Vector3D(0,0,1,Vector3D::POINT3DCART), radius_ );
}

int GLSphereFactory::size() const
{
  return 3*OpenGLUtility::sizeCircle();
}

GLLineFactory::GLLineFactory( const Vector3D& begin, const Vector3D& end )
: begin_( begin ), end_( end )
{
}

void GLLineFactory::create() const
{
  glPushMatrix();
  glBegin( GL_LINES );
  glVertex3f( begin_.x(), begin_.y(), begin_.z() );
  glVertex3f( end_.x(),   end_.y(),   end_.z() );
  glEnd();
  glPopMatrix();
}

int GLLineFactory::size() const
{
  return 6;
}

