#ifndef SPHERE3D_H
#define SPHERE3D_H

#include "dllspecutility.h"
#include "vector3d.h"
#include "line3d.h"

#include <QList>

class DLL_EXPORT_UTILITY Sphere3D
{
  public:
    Sphere3D(){}
    Sphere3D( const Vector3D& center, float radius );
    QList< Vector3D > intersect( const Line3D& line ) const;

    Vector3D center() const;
    float radius() const;

  private:
    Vector3D center_;
    float radius_;
};

#endif