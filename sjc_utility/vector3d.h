#ifndef Vector3D_H
#define Vector3D_H

#include "dllspecutility.h"
#include <QVector>
#include <QMetaType>
#include <cassert>

class DLL_EXPORT_UTILITY Vector3D
{
  public:
    enum Vector3DType { POINT3DCART, POINT3DPOLE, VECTOR3D };
    Vector3D();
    Vector3D( float xRho, float yPhi, float zTheta, Vector3DType type );
    Vector3D( const Vector3D& other );
    ~Vector3D();

    float x() const;
    float y() const;
    float z() const;
    float d() const;
    
    float rho() const;
    float phi() const;
    float theta() const;

    float dotProduct( const Vector3D& other ) const;
    Vector3D crossProduct( const Vector3D& other ) const;
    float angle( const Vector3D& other ) const;

    float length() const;
    float distance( const Vector3D& other ) const;
    Vector3D unitVector() const;
    bool isVector() const;

    Vector3D rotate( const Vector3D& axis, double angle );

    Vector3D operator-( const Vector3D& other ) const;
    Vector3D operator+( const Vector3D& other ) const;

    Vector3D operator/( float other ) const;
    Vector3D operator*( float other ) const;
    bool     operator==( const Vector3D& other ) const;
    bool     operator!=( const Vector3D& other ) const;

    operator QVariant() const;
    QString  toString() const;

  private:
    QVector< float > vec_;
};

Q_DECLARE_METATYPE( Vector3D );

#endif