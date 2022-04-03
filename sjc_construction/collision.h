#ifndef COLLISION_H
#define COLLISION_H

#include "pole.h"

class Plane3D;

class DLL_EXPORT_CONSTRUCTION Collision
{
  public:
    Collision( const Pole& solid, const Pole& moving );
    Plane3D solution() const;
    //Collision& operator=( const Collision& other );

  private:
    Pole solid_;
    Pole moving_;
};

class DLL_EXPORT_CONSTRUCTION RotateCollision
{
  public:
    RotateCollision( const Pole& solid, const Pole& moving );
    Plane3D solution( const Pole& movingOriginal ) const;
    //Collision& operator=( const Collision& other );

  private:
    Pole solid_;
    Pole moving_;
};

#endif