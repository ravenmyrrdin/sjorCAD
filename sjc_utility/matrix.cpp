#include "matrix.h"
#include <QtTest/QtTest>

#include "vector3d.h"

Matrix::Matrix( const QVector< QVector < float > >& data )
{
  data_ = data;
}

Matrix::Matrix( float data[], int rows, int columns )
{
  for( int j = 0; j < columns; ++j )
  {
    QVector< float > r;
    for( int i = 0; i < rows; ++i )
      r << data[ i + j*columns ];
    data_ << r;
  }
}

float Matrix::determinant()
{
  if( data_.size() == 1 )
    return data_[ 0 ][ 0 ];

  float result = 0;
  for( int i = 0; i < data_.size(); ++i )
    result += data_[ 0 ][ i ] * matrixWithoutRow( 0 ).matrixWithoutColumn( i ).determinant() * (i%2?-1:1);
  //qDebug() << "determinant:" << toString() << result;
  return result;
}

Matrix Matrix::matrixWithoutRow( int row )
{
  QVector< QVector < float > > newdata;
  for( int i = 0; i < data_.size(); ++i )
  {
    if( i != row )
      newdata << data_[ i ];
  }
  return newdata;
}

Matrix Matrix::matrixWithoutColumn( int column )
{
  QVector< QVector< float > > newdata;
  for( QVector< QVector< float > >::const_iterator iter = data_.constBegin(); iter != data_.constEnd(); ++iter )
  {
    QVector< float > newrow = *iter;
    newrow.remove( column );
    newdata << newrow;
  }
  return newdata;
}

QString Matrix::toString()
{
  QString result = "\n";
  for( QVector< QVector< float > >::const_iterator iter = data_.constBegin(); iter != data_.constEnd(); ++iter )
  {
    for( QVector< float >::const_iterator iter2 = (*iter).constBegin(); iter2 != (*iter).constEnd(); ++iter2 )
      result += QString::number( *iter2 ) + " ";
    result += "\n";
  }
  return result;
}

Vector3D Matrix::mult( const Vector3D& p ) const
{
  float x = data_[ 0 ][ 0 ] * p.x() + data_[ 0 ][ 1 ] * p.y() + data_[ 0 ][ 2 ] * p.z() + data_[ 0 ][ 3 ] * (p.isVector() ? 0 : 1);
  float y = data_[ 1 ][ 0 ] * p.x() + data_[ 1 ][ 1 ] * p.y() + data_[ 1 ][ 2 ] * p.z() + data_[ 1 ][ 3 ] * (p.isVector() ? 0 : 1);
  float z = data_[ 2 ][ 0 ] * p.x() + data_[ 2 ][ 1 ] * p.y() + data_[ 2 ][ 2 ] * p.z() + data_[ 2 ][ 3 ] * (p.isVector() ? 0 : 1);

  return Vector3D( x, y, z, p.isVector() ? Vector3D::VECTOR3D : Vector3D::POINT3DCART );
}
