
#include <cmath>
#include "vector3d.h"
#include "matrix.h"
#include <QVariant>

Vector3D::~Vector3D(){}

Vector3D::Vector3D()
{
  vec_ << 0 << 0 << 0 << 1;
}

Vector3D::Vector3D( float xRho, float yPhi, float zTheta, Vector3DType type )
{
  if( type == POINT3DCART )
    vec_ << xRho << yPhi << zTheta << 1;
  else if( type == VECTOR3D )
    vec_ << xRho << yPhi << zTheta << 0;
  else if( type == POINT3DPOLE )
    vec_ << xRho * std::sin( yPhi ) * std::cos( zTheta )
         << xRho * std::sin( yPhi ) * std::sin( zTheta )
         << xRho * std::cos( yPhi ) << 1;
}

Vector3D::Vector3D( const Vector3D& other )
: vec_( other.vec_ )
{}

float Vector3D::x() const
{
  return !isVector() ? vec_[ 0 ] / vec_[ 3 ] : vec_[ 0 ];
}

float Vector3D::y() const
{
  return !isVector() ? vec_[ 1 ] / vec_[ 3 ] : vec_[ 1 ];
}

float Vector3D::z() const
{
  return !isVector() ? vec_[ 2 ] / vec_[ 3 ] : vec_[ 2 ];
}

float Vector3D::d() const
{
  return vec_[ 3 ];
}

float Vector3D::rho() const
{
  return std::sqrt( x()*x() + y()*y() + z()*z() );
}

float Vector3D::phi() const
{
  return std::acos( z() / rho() );
}

float Vector3D::theta() const
{
  if( qFuzzyCompare( x(), 0.0f ) && qFuzzyCompare( y(), 0.0f ) )
    return 0;

  if( x() >= 0 )
    return std::asin( y() / std::sqrt( x()*x() + y()*y() ) );
  else
    return 3.1415f - std::asin( y() / std::sqrt( x()*x() + y()*y() ) );
}

float Vector3D::length() const
{
  return std::sqrt( x()*x() + y()*y() + z()*z() );
}

float Vector3D::distance( const Vector3D& other ) const
{
  return ((*this)-other).length();
}

Vector3D Vector3D::unitVector() const
{
  return *this / length();
}

bool Vector3D::isVector() const
{
  return qFuzzyCompare( vec_[ 3 ], 0.0f );
}

Vector3D Vector3D::rotate( const Vector3D& axis, double angle )
{
  double c = cos( angle );
  double s = sin( angle );
  double t = 1 - cos( angle );

  float x = axis.x();
  float y = axis.y();
  float z = axis.z();

  QVector< float > row1 = QVector< float >( 4 );
  QVector< float > row2 = QVector< float >( 4 );
  QVector< float > row3 = QVector< float >( 4 );
  QVector< float > row4 = QVector< float >( 4 );

  row1[ 0 ] = t*x*x+c;       row1[ 1 ] = t*x*y-s*z;        row1[ 2 ] = t*x*z+s*y;        row1[ 3 ] = 0;
  row2[ 0 ] = t*x*y+s*z;     row2[ 1 ] = t*y*y+c;          row2[ 2 ] = t*y*z-s*x;        row2[ 3 ] = 0;
  row3[ 0 ] = t*x*z-s*y;     row3[ 1 ] = t*y*z+s*x;        row3[ 2 ] = t*z*z+c;          row3[ 3 ] = 0;
  row4[ 0 ] = 0;             row4[ 1 ] = 0;                row4[ 2 ] = 0;                row4[ 3 ] = 1;

  Matrix m( QVector< QVector< float > >() << row1 << row2 << row3 << row4 );
  (*this) = m.mult( *this );
  
  return *this;
}

Vector3D Vector3D::operator-( const Vector3D& other ) const
{
  return Vector3D( x() - other.x(), y() - other.y(), z() - other.z(), ( isVector() || other.isVector() ) ? POINT3DCART : VECTOR3D );
}

Vector3D Vector3D::operator+( const Vector3D& other ) const
{
  return Vector3D( x() + other.x(), y() + other.y(), z() + other.z(), ( isVector() || other.isVector() ) ? POINT3DCART : VECTOR3D );
}

Vector3D Vector3D::operator/( float other ) const
{
  return Vector3D( x() / other, y() / other, z() / other, isVector() ? VECTOR3D : POINT3DCART );
}

Vector3D Vector3D::operator*( float other ) const
{
  return Vector3D( x() * other, y() * other, z() * other, isVector() ? VECTOR3D : POINT3DCART );
}

bool Vector3D::operator==( const Vector3D& other ) const
{
  return qFuzzyCompare( x(), other.x() ) && qFuzzyCompare( y(), other.y() ) && qFuzzyCompare( z(), other.z() ) && isVector() == other.isVector();
}

bool Vector3D::operator!=( const Vector3D& other ) const
{
  return !( *this == other );
}

Vector3D::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

QString Vector3D::toString() const
{
  return QString( "(%1,%2,%3,%4 )" ).arg( QString::number( x(), 'f', 3 ) ).arg( QString::number( y(), 'f', 3 ) ).arg( QString::number( z(), 'f', 3 ) ).arg( (int)!isVector() );
}

float Vector3D::dotProduct( const Vector3D& other ) const
{
  return x()*other.x() + y()*other.y() + z()*other.z();
}

Vector3D Vector3D::crossProduct( const Vector3D& other ) const
{
  return Vector3D( y()*other.z() - z()*other.y(),
                  -x()*other.z() + z()*other.x(),
                   x()*other.y() - y()*other.x(), VECTOR3D );
}


float Vector3D::angle( const Vector3D& other ) const
{
  return acos( other.unitVector().dotProduct( unitVector() ) );
}
