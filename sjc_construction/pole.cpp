#include "pole.h"
#include "mathematics.h"
#include <cmath>

#include <QtTest/QtTest>

long Pole::gId_ = 0;

Pole::Pole()
: id_( gId_++ ), radius_( 0 )
{
}

Pole::~Pole()
{
}

Pole::Pole( Vector3D center, Vector3D vector, float radius )
: center_( center ), vector_( vector ), radius_( radius ), id_( gId_++ )
{
}

/*Pole& Pole::operator=( const Pole& other )
{
  center_ = other.center_;
  vector_ = other.vector_;
  radius_ = other.radius_;

  return *this;
}*/

Vector3D Pole::center() const
{
  return center_;
}

Vector3D Pole::begin() const
{
  return center_ - vector_ / 2;
}

Vector3D Pole::end() const
{
  return center_ + vector_ / 2;
}

Vector3D Pole::direction() const
{
  return vector_.unitVector();
}

Line3D Pole::line() const
{
  return Line3D( begin(), end() );
}

float Pole::length() const
{
  return vector_.length();
}

float Pole::radius() const
{
  return radius_;
}

long Pole::id() const
{
  return id_;
}

bool Pole::setLength( float l )
{
  if( l < 0 || qFuzzyCompare( l, 0.0f ) )
    return false;

  return true;
}

bool Pole::setRadius( float r )
{
  if( r < 0 || qFuzzyCompare( r, 0.0f ) || r > 1 )
    return false;

  radius_ = r;
  return true;
}

bool Pole::setCenter( const Vector3D& center )
{
  if( center.isVector() )
    return false;

  center_ = center;
  return true;
}

bool Pole::setDirection( const Vector3D& direction )
{
  if( !direction.isVector() )
    return false;

  vector_ = Vector3D( direction.unitVector() * vector_.length() );
  return true;
}

/*bool Pole::contains( const Vector3D& point ) const
{
  Vector3D pointRelToPoleCenter = point - center();
  Vector3D projectedOnPole =   direction() *  pointRelToPoleCenter.length();
  Vector3D projectedOnPole2 =  direction() * -pointRelToPoleCenter.length();

  qDebug() << (projectedOnPole -pointRelToPoleCenter).length() << (projectedOnPole2-pointRelToPoleCenter).length();

  return std::min( (projectedOnPole -pointRelToPoleCenter).length(), 
                   (projectedOnPole2-pointRelToPoleCenter).length() ) < radius();
}*/

Vector3D Pole::closestPointInPoleToPoint( const Vector3D& from )
{
  return line().shortestSegment( from ).begin();
}

Vector3D Pole::closestPointInPoleToLine( const Vector3D& a, const Vector3D& b )
{
  return line().shortestSegment( Line3D( a, b ) ).begin();
}

Vector3D Pole::closestPointOnPoleToLine( const Vector3D& a, const Vector3D& b )
{
  Line3D segment = line().shortestSegment( Line3D( a, b ) );
  return segment.begin() + segment.unitVector() * radius();
}

void Pole::rotate( const Line3D& axis, double angle )
{
  // TODO translate center to the axis, currently we assert that the axis passes through the center
  vector_.rotate( axis.unitVector(), angle );
}

bool Pole::collides( const Pole& other ) const
{
  Line3D shortest = line().shortestSegment( other.line() );
  double distanceown   = Vector3D( center()       - shortest.begin() ).length();
  double distanceother = Vector3D( other.center() - shortest.end()   ).length();
  return distanceown < length()/2.0 && distanceother < other.length()/2.0 && shortest.length() < (radius()+other.radius());
}
