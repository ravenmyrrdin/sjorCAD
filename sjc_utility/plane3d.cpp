#include "plane3d.h"
#include "matrix.h"
#include <QtTest/QtTest>
#include <cassert>

Plane3D::Plane3D( const Vector3D& p1, const Vector3D& p2, const Vector3D& p3 )
: p1_( p1 ), p2_( p2 ), p3_( p3 )
{
}

Plane3D::Plane3D( const Line3D& l1, const Line3D& l2 )
: p1_( l1.begin() ), p2_( l1.end() ), p3_( l2.end() )
{
  assert( !l1.parallel( l2 ) );
  assert( p1_ != p2_ );
  assert( p1_ != p3_ );
  assert( p2_ != p3_ );
}

Vector3D Plane3D::intersect( const Line3D& line ) const
{
  // http://mathworld.wolfram.com/Line-PlaneIntersection.html

  float top[ 16 ] = {
    1,       1,       1,       1,
    p1_.x(), p2_.x(), p3_.x(), line.begin().x(),
    p1_.y(), p2_.y(), p3_.y(), line.begin().y(),
    p1_.z(), p2_.z(), p3_.z(), line.begin().z()
  };

  float bottom[ 16 ] = {
    1,       1,       1,       0,
    p1_.x(), p2_.x(), p3_.x(), line.unitVector().x(),
    p1_.y(), p2_.y(), p3_.y(), line.unitVector().y(),
    p1_.z(), p2_.z(), p3_.z(), line.unitVector().z()
  };
  
  //qDebug() << Matrix( top, 4, 4 ).determinant();
  //qDebug() << Matrix( bottom, 4, 4 ).determinant();
  float t = -1 * Matrix( top, 4, 4 ).determinant() / Matrix( bottom, 4, 4 ).determinant();
  return line.begin() + line.unitVector() * t;
}

// http://www.koders.com/cpp/fid073EDD7A6ABCDD61FF7C128FE3563F9C75AD61ED.aspx
Line3D Plane3D::intersect( const Plane3D& p ) const
{
  Vector3D n0 = normal();
  Vector3D n1 = p.normal();
  float n00 = n0.dotProduct( n0 );
  float n01 = n0.dotProduct( n1 );
  float n11 = n1.dotProduct( n1 );
  float det = n00 * n11 - n01 * n01;

  float c0 = ( n11 * d()   - n01 * p.d() ) / det;
  float c1 = ( n00 * p.d() - n01 * d()   ) / det;
  Vector3D a = n0 * c0 + n1 * c1;
  return Line3D( a, a + n0.crossProduct( n1 ) );
}

Vector3D Plane3D::closestPointTo( const Vector3D& p ) const
{
  Line3D line( p, p+normal() );
  return intersect( line );
}

Vector3D Plane3D::normal() const
{
  return Vector3D( p2_ - p1_ ).crossProduct( p3_ - p1_ );
}

float Plane3D::d() const
{
  Vector3D n = normal();
  return n.x() * p1_.x() + n.y() * p1_.y() + n.z() * p1_.z();
}
