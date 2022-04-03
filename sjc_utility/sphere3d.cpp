#include "sphere3d.h"
#include <cmath>

Sphere3D::Sphere3D( const Vector3D& center, float radius )
: center_( center ), radius_( radius )
{
}

QList< Vector3D > Sphere3D::intersect( const Line3D& line ) const
{
  Vector3D p1 = line.begin();
  Vector3D p2 = line.end();
  Vector3D dp = p2-p1;

  double a,b,c;
  double bb4ac;

  a = dp.x() * dp.x() + dp.y() * dp.y() + dp.z() * dp.z();
  b = 2 * (dp.x() * (p1.x() - center_.x()) + dp.y() * (p1.y() - center_.y()) + dp.z() * (p1.z() - center_.z()));
  c = center_.x() * center_.x() + center_.y() * center_.y() + center_.z() * center_.z();
  c += p1.x() * p1.x() + p1.y() * p1.y() + p1.z() * p1.z();
  c -= 2 * (center_.x() * p1.x() + center_.y() * p1.y() + center_.z() * p1.z());
  c -= radius_ * radius_;
  bb4ac = b * b - 4 * a * c;
  QList< Vector3D > results;
  if( qFuzzyCompare( a, 0.0 ) || bb4ac < 0 ) 
    return results;
  else if( qFuzzyCompare( bb4ac, 0.0 ) )
  {
    double mu = -b / 2*a;
    results << p1 + (p2-p1)*mu;
    return results;
  }
  else
  {
    double mu1 = (-b + sqrt(bb4ac)) / (2 * a);
    double mu2 = (-b - sqrt(bb4ac)) / (2 * a);

    results << p1 + (p2-p1)*mu1;
    results << p1 + (p2-p1)*mu2;

    return results;
  }
}

Vector3D Sphere3D::center() const
{
  return center_;
}

float Sphere3D::radius() const
{
  return radius_;
}


/*
   Calculate the intersection of a ray and a sphere
   The line segment is defined from p1 to p2
   The sphere is of radius r and centered at sc
   There are potentially two points of intersection given by
   p = p1 + mu1 (p2 - p1)
   p = p1 + mu2 (p2 - p1)
   Return FALSE if the ray doesn't intersect the sphere.
*/
//int RaySphere(XYZ p1,XYZ p2,XYZ sc,double r,double *mu1,double *mu2)
//{
//   double a,b,c;
//   double bb4ac;
//   XYZ dp;
//
//   dp.x = p2.x - p1.x;
//   dp.y = p2.y - p1.y;
//   dp.z = p2.z - p1.z;
//   a = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
//   b = 2 * (dp.x * (p1.x - sc.x) + dp.y * (p1.y - sc.y) + dp.z * (p1.z - sc.z));
//   c = sc.x * sc.x + sc.y * sc.y + sc.z * sc.z;
//   c += p1.x * p1.x + p1.y * p1.y + p1.z * p1.z;
//   c -= 2 * (sc.x * p1.x + sc.y * p1.y + sc.z * p1.z);
//   c -= r * r;
//   bb4ac = b * b - 4 * a * c;
//   if (ABS(a) < EPS || bb4ac < 0) {
//      *mu1 = 0;
//      *mu2 = 0;
//      return(FALSE);
//   }
//
//   *mu1 = (-b + sqrt(bb4ac)) / (2 * a);
//   *mu2 = (-b - sqrt(bb4ac)) / (2 * a);
//
//   return(TRUE);
//}
