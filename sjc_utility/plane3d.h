#ifndef PLANE3D_H
#define PLANE3D_H

#include "dllspecutility.h"
#include "vector3d.h"
#include "line3d.h"

class DLL_EXPORT_UTILITY Plane3D
{
  public:
    Plane3D( const Vector3D& p1, const Vector3D& p2, const Vector3D& p3 );
    Plane3D( const Line3D& l1, const Line3D& l2 );

    Vector3D intersect( const Line3D& line ) const;
    Line3D   intersect( const Plane3D& p ) const;
    Vector3D closestPointTo( const Vector3D& ) const;
    Vector3D normal() const;
    float    d() const;

  private:
    Vector3D p1_;
    Vector3D p2_;
    Vector3D p3_;
};

#endif