#include "line3d.h"
#include "mathematics.h"


Line3D::Line3D()
{}

Line3D::Line3D( const Vector3D& begin, const Vector3D& end )
: begin_( begin ), end_( end )
{
}

Vector3D Line3D::begin() const
{
  return begin_;
}

Vector3D Line3D::end() const
{
  return end_;
}

Vector3D Line3D::unitVector() const
{
  return ( end_ - begin_ ).unitVector();
}

double Line3D::length() const
{
  return ( end_ - begin_ ).length();
}

bool Line3D::isPoint() const
{
  return begin_ == end_;
}

bool Line3D::parallel( const Line3D& other ) const
{
  return unitVector() == other.unitVector() || unitVector() == other.unitVector() * -1;
}

bool Line3D::intersects( const Line3D& other ) const
{
  return !parallel( other ) && shortestSegment( other ).isPoint();
}

bool Line3D::crosses( const Line3D& other ) const
{
  return !parallel( other ) && !intersects( other );
}

/*bool Line3D::contains( const Vector3D& p ) const
{
  double t1 = 
}*/

Line3D Line3D::shortestSegment( const Line3D& other ) const
{
  XYZ poleA( begin().x(),             begin().y(),           begin().z() );
  XYZ poleB( end().x(),               end().y(),             end().z() );
  XYZ lineA( other.begin().x(),       other.begin().y(),     other.begin().z() );
  XYZ lineB( other.end().x(),         other.end().y(),       other.end().z() );

  XYZ resultA;
  XYZ resultB;
  double a, b;
  if( LineLineIntersect( poleA, poleB, lineA, lineB, &resultA, &resultB, &a, &b ) )
  {
    Vector3D resultAVec( resultA.x, resultA.y, resultA.z, Vector3D::POINT3DCART );
    Vector3D resultBVec( resultB.x, resultB.y, resultB.z, Vector3D::POINT3DCART );
    return Line3D( resultAVec, resultBVec );
  }
  else
    return Line3D();
}

Line3D Line3D::shortestSegment( const Vector3D& point ) const
{
  Vector3D beginToPoint = point - begin();
  Vector3D projectedRelOnBalk = unitVector() * beginToPoint.length() * std::cos( beginToPoint.angle( unitVector() ) );
  return Line3D( begin() + projectedRelOnBalk, point );
}

Vector3D Line3D::closestPointTo( const Vector3D& point ) const
{
  return shortestSegment( point ).begin();
}

Vector3D Line3D::intersection( const Line3D& other ) const
{
  /*
  http://mathforum.org/library/drmath/view/62814.html
  Let's try this with vector algebra. First write the two equations like 
  this.

    L1 = P1 + a V1

    L2 = P2 + b V2

  P1 and P2 are points on each line. V1 and V2 are the direction vectors 
  for each line.

  If we assume that the lines intersect, we can look for the point on L1 
  that satisfies the equation for L2. This gives us this equation to 
  solve.

    P1 + a V1 = P2 + b V2

  Now rewrite it like this.

    a V1 = (P2 - P1) + b V2

  Now take the cross product of each side with V2. This will make the 
  term with 'b' drop out.

    a (V1 X V2) = (P2 - P1) X V2
  */
  if( intersects( other ) )
  {
    if( begin_ == other.begin() )
      return begin_;
    else
    {
      double a = ( other.begin() - begin_ ).crossProduct( other.unitVector() ).length() / unitVector().crossProduct( other.unitVector() ).length();
      return begin_ + unitVector() * a;
    }
  }
  else
    return Vector3D();
}

double Line3D::angle( const Line3D& other ) const
{
  return acos( this->unitVector().dotProduct( other.unitVector() ) );
}

void Line3D::setCenter( const Vector3D& center )
{
  Vector3D trans = center - (begin_+end_)/2.0;
  begin_ = begin_ + trans;
  end_   = end_   + trans;
}

bool Line3D::operator==( const Line3D& other )
{
  return begin_ == other.begin() && end_ == other.end();
}
