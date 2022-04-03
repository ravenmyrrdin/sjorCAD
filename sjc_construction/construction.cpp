#include "construction.h"
#include <QtTest/QtTest>

#include "collision.h"
#include "plane3d.h"

Poles::Poles()
{
}

int Poles::columnCount( const QModelIndex& index ) const
{
  return 7;
}

int Poles::rowCount( const QModelIndex& index ) const
{
  if( index == QModelIndex() )
    return poles_.size();
  else
    return 0;
}

Qt::ItemFlags Poles::flags( const QModelIndex & index ) const
{
  if( index.isValid() )
  {
    if( index.column() == 3 || index.column() == 4 )
      return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
      return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  }
  else
    return 0;
}

QVariant Poles::data( const QModelIndex& index, int role ) const
{
  /*if( role != Qt::DisplayRole && role != Qt::TextColorRole )
    return QVariant();
  if( role == Qt::TextColorRole )
    return QColor( Qt::black );*/
  switch( role )
  {
  case Qt::SizeHintRole:         return QVariant();
  case Qt::FontRole:             return QFont();
  case Qt::TextAlignmentRole:    return Qt::AlignLeft;
  case Qt::BackgroundColorRole:  return QColor( Qt::white );
  case Qt::TextColorRole:        return QColor( Qt::black );
  case Qt::CheckStateRole:       return QVariant();
  }

  const PoleData* poledata = static_cast< const PoleData* >( index.internalPointer() );
  
  switch( index.column() )
  {
  case 0: return poledata->pole_->id();
  case 1: return poledata->pole_->center().toString();
  case 2: return poledata->pole_->direction().toString();
  case 3: return poledata->pole_->length();
  case 4: return poledata->pole_->radius();
  case 5: return poledata->color_;
  case 6: return QColor( Qt::yellow );
  default: return QVariant();
  }
}

bool Poles::setData( const QModelIndex& index, const QVariant& data, int role )
{
  PoleData* poledata = static_cast< PoleData* >( index.internalPointer() );
  bool success = false;
  QVariant oldData = this->data( index, role );

  switch( index.column() )
  {
  //case 0: return poledata->pole_->id();
  case 1: success = poledata->pole_->setCenter( data.value< Vector3D >() ); break;
  case 2: success = poledata->pole_->setDirection( data.value< Vector3D >() ); break;
  case 3: success = poledata->pole_->setLength( data.value< float >() ); break;
  case 4: success = poledata->pole_->setRadius( data.value< float >() ); break;
  //case 5: return poledata->color_;
  //case 6: return QColor( Qt::yellow );
  }

  if( success && oldData != this->data( index, role ) )
    emit dataChanged( index, index );
  return success;
}

QModelIndex Poles::index( int row, int column, const QModelIndex& parent ) const
{
  if( row < 0 || column < 0 )
    return QModelIndex(); // invalid request

  if( parent == QModelIndex() && row < rowCount() )
    return createIndex( row, column, poles_[ row ] );
  else
    return QModelIndex(); // atm this is only a root with a list, not a tree
}

QModelIndex Poles::parent( const QModelIndex& index ) const
{
  return QModelIndex();
}

bool Poles::hasChildren( const QModelIndex& index ) const
{
  return index == QModelIndex();
}

/*QVariant Poles::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( orientation == Qt::Vertical )
  {
    switch( section )
    {
    case 0: return "ID";
    case 1: return "Center";
    case 2: return "Direction";
    case 3: return "Length";
    case 4: return "Radius";
    default: return "";
    }
  }
  else
    return QString();
}*/

QModelIndex Poles::createPole( const Vector3D& center, const Vector3D& vector, float radius, const QColor& color )
{
  beginInsertRows( QModelIndex(), rowCount(), rowCount() );
  poles_.push_back( new PoleData( new Pole( center, vector, radius ), color ) );
  qDebug() << "created poledata: " << poles_.back();
  endInsertRows();
  return index( rowCount()-1, 0 );
}

QList< Collision > Poles::rawCollisions( const Pole& destinationPole ) const
{
  QList< Collision > collisions;
  for( QList< PoleData* >::const_iterator iter = poles_.constBegin(); iter != poles_.constEnd(); ++iter )
  {
    if( destinationPole.id() != (*iter)->pole_->id() && destinationPole.collides( (*(*iter)->pole_) ) )
      collisions << Collision( (*(*iter)->pole_), destinationPole );
  }
  return collisions;
}

PoleData* Poles::pole      ( int id ) const
{
  for( QList< PoleData* >::const_iterator iter = poles_.constBegin(); iter != poles_.constEnd(); ++iter )
  {
    if( (*iter)->pole_->id() == id )
      return (*iter);
  }
  return 0;
}

