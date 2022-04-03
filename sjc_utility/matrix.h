#ifndef MATRIX_H
#define MATRIX_H

#include "dllspecutility.h"
#include <QVector>

class Vector3D;

class DLL_EXPORT_UTILITY Matrix
{
public:
  Matrix( float data[], int rows, int columns );
  Matrix( const QVector< QVector < float > >& data );
  float determinant();
  Matrix matrixWithoutRow( int row );
  Matrix matrixWithoutColumn( int column );
  QString toString();
  Vector3D mult( const Vector3D& p ) const;

private:
  QVector< QVector < float > > data_;
};

#endif