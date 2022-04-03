#ifndef POLE_H
#define POLE_H

#include "dllspecconstruction.h"
#include "vector3d.h"
#include "line3d.h"

class Pole;

class DLL_EXPORT_CONSTRUCTION Pole
{
  public:
    Pole();
    virtual ~Pole();
    Pole( Vector3D center, Vector3D vector, float radius );
    //Pole( const Pole& );
    //Pole& operator=( const Pole& );
    Vector3D center()    const;
    Vector3D begin()     const;
    Vector3D end()       const;
    Vector3D direction() const;
    Line3D   line()      const;

    float    length()    const;
    float    radius()    const;

    long     id()        const;

    bool setLength( float l );
    bool setRadius( float r );
    bool setCenter( const Vector3D& center );
    bool setDirection( const Vector3D& direction );

    //bool contains( const Vector3D& point ) const;
    Vector3D closestPointInPoleToPoint( const Vector3D& from );
    Vector3D closestPointInPoleToLine( const Vector3D& a, const Vector3D& b );
    Vector3D closestPointOnPoleToLine( const Vector3D& a, const Vector3D& b );

    void rotate( const Line3D& axis, double angle );
    bool collides( const Pole& other ) const;


  private:
    //Line3D line_;
    Vector3D center_;
    Vector3D vector_;
    float radius_;
    long id_;
    static long gId_;
};

#endif
