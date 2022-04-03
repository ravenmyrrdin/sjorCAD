#include "collision.h"

#include "plane3d.h"
#include "pole.h"



Collision::Collision( const Pole& solid, const Pole& moving )
: solid_( solid ),
moving_( moving )
{
}

/*Collision& Collision::operator=( const Collision& other )
{
  solid_ = other.solid_;
  moving_ = other.moving_;
  return *this;
}*/

Plane3D Collision::solution() const
{
  Line3D orthoSegment = solid_.line().shortestSegment( moving_.line() );
  if( qFuzzyCompare( orthoSegment.length(), 0.0 ) )
    orthoSegment = Line3D( orthoSegment.begin(), orthoSegment.begin() + solid_.direction().crossProduct( moving_.direction() ) );
  orthoSegment = Line3D( orthoSegment.begin(), orthoSegment.begin() + orthoSegment.unitVector() * ( solid_.radius() + moving_.radius() ) );
  Line3D line1 = solid_.line();
  Line3D line2 = moving_.line();
  
  line1.setCenter( orthoSegment.end() );
  line2.setCenter( orthoSegment.end() );

  return Plane3D( line1, line2 );
}


RotateCollision::RotateCollision( const Pole& solid, const Pole& moving )
: solid_( solid ),
moving_( moving )
{
}
/*
Plane3D RotateCollision::solution( const Pole& movingOriginal, const Vector3D& r ) const
{
  Plane pl = Plane( moving_.line(), movingOriginal.line() );
  Vector3D p = pl.intersect( solid_ );
  (p-r).rotate( 
}*/

