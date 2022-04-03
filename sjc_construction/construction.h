#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include "dllspecconstruction.h"

#include <QAbstractItemModel>
#include <QAbstractItemView>
#include "pole.h"
#include <QList>

class MyGLWidget;
class Collision;

struct PoleData
{
  PoleData( Pole* pole, const QColor& color )
    : pole_( pole ), color_( color ){}
  Pole* pole_;
  QColor color_;
};

class DLL_EXPORT_CONSTRUCTION Poles : public QAbstractItemModel
{
  public:
    Poles();
    virtual int           columnCount( const QModelIndex& index = QModelIndex() ) const;
    virtual int           rowCount   ( const QModelIndex& index = QModelIndex() ) const;
    virtual Qt::ItemFlags flags      ( const QModelIndex & index ) const;
    virtual QVariant      data       ( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual bool          setData    ( const QModelIndex& index, const QVariant& data, int role = Qt::EditRole );
    virtual QModelIndex   index      ( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex   parent     ( const QModelIndex& index = QModelIndex() ) const;
    virtual bool          hasChildren( const QModelIndex& index ) const;
    //virtual QVariant      headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    QModelIndex           createPole( const Vector3D& center, const Vector3D& vector, float radius, const QColor& color );

    QList< Collision >    rawCollisions( const Pole& destinationPole ) const;
    PoleData*             pole      ( int id ) const;

  private:
    QList< PoleData* > poles_;
};



#endif