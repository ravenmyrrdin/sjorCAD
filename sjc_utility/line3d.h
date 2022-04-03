#ifndef LINE3D_H
#define LINE3D_H

#include "dllspecutility.h"
#include "vector3d.h"

class DLL_EXPORT_UTILITY Line3D
{
public:
  Line3D();
  Line3D( const Vector3D& begin, const Vector3D& end );
  Vector3D begin() const;
  Vector3D end() const;
  Vector3D unitVector() const;
  double length() const;
  
  bool isPoint() const;
  bool parallel( const Line3D& other ) const;
  bool intersects( const Line3D& other ) const;
  bool crosses( const Line3D& other ) const;
  bool contains( const Vector3D& p ) const;

  Line3D shortestSegment( const Line3D& other ) const;
  Line3D shortestSegment( const Vector3D& point ) const;
  Vector3D closestPointTo( const Vector3D& point ) const;
  Vector3D intersection( const Line3D& other ) const;
  double angle( const Line3D& other ) const;

  void setCenter( const Vector3D& center );

  bool operator==( const Line3D& other );
  

private:
  Vector3D begin_;
  Vector3D end_;

};

Q_DECLARE_METATYPE( Line3D );

#endif