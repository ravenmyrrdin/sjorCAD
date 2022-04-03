#include "glconstructionview.h"
#include "construction.h"
#include "glfactory.h"
#include <QtTest/QtTest>

ConstructionView::ConstructionView( MyGLWidget* glwidget, QWidget* parent ) 
: QAbstractItemView( parent ), 
glwidget_( glwidget ), 
poleMoveDelegate_( new GLPoleMoveDelegate( glwidget ) ),
poleRotateDelegate_( new GLPoleRotateDelegate( glwidget ) ),
poleeditor_( 0 )
{
  // whenever viewobjects is accessed with a null-ptr, return an invalid index
  viewobjects_[ 0 ] = QModelIndex();
  // create the ground
  glwidget->createObject( GLPlaneFactory( Vector3D(), 5, QColor( Qt::white ) ) );

  connect( glwidget_, SIGNAL( objectSelected( GLObject* ) ),                this, SLOT( selectObject( GLObject* ) ) );
  connect( glwidget_, SIGNAL( objectAboutToMove( GLObject*, Vector3D ) ),   this, SLOT( aboutToMoveObject( GLObject*, Vector3D ) ) );
  connect( glwidget_, SIGNAL( objectMoved() ),                              this, SLOT( finishMovingObject() ) );
  connect( glwidget_, SIGNAL( objectAboutToRotate( GLObject*, Vector3D ) ), this, SLOT( aboutToRotateObject( GLObject*, Vector3D ) ) );
  connect( glwidget_, SIGNAL( objectRotated() ),                            this, SLOT( finishRotatingObject() ) );
}

QModelIndex ConstructionView::indexAt( const QPoint& point ) const
{ 
  const GLObject* globject = glwidget_->objectUnderPixel( point );
  return viewobjects_[ globject ];
}

void ConstructionView::rowsInserted( const QModelIndex& parent, int start, int end )
{
  assert( start == end );
  PoleData* poledata = static_cast< PoleData* >( model()->index( start, 0, parent ).internalPointer() );
  assert( poledata && poledata->pole_ );
  //qDebug() << "poledata inserted: " << poledata;
  const GLObject* globject = glwidget_->createObject( GLPoleFactory( *poledata->pole_, poledata->color_ ) );
  viewobjects_[ globject ] = QPersistentModelIndex( model()->index( start, 0, parent ) );
  glwidget_->paintGL();
}

void ConstructionView::selectObject( GLObject* globject )
{
  if( globject && viewobjects_.contains( globject ) )
    selectionModel()->select( viewobjects_[ globject ], QItemSelectionModel::ClearAndSelect );
  else
    selectionModel()->select( QModelIndex(), QItemSelectionModel::ClearAndSelect );
}

void ConstructionView::aboutToMoveObject( GLObject* globject, Vector3D grabPoint )
{
  //qDebug() << "aboutToMoveObject" << globject;
  if( globject 
    && viewobjects_[ globject ] == selectionModel()->selectedIndexes().first() )
    //&& static_cast< PoleData* >( viewobjects_[ globject ].internalPointer() )->pole_->contains( grabPoint ) )
  {
    viewmode_ = MOVEPOLE;
    assert( !poleeditor_ );
    poleeditor_ = poleMoveDelegate_->createEditor( 0, QStyleOptionViewItem(), viewobjects_[ globject ] );
    poleMoveDelegate_->setEditorData( poleeditor_, viewobjects_[ globject ] );
    poleMoveDelegate_->setEditorMouse( poleeditor_, grabPoint );
    connect( glwidget_, SIGNAL( mouseMoved( QPoint, MyGLWidget::GLMode ) ), poleeditor_, SLOT( updateEvent( QPoint, MyGLWidget::GLMode ) ) );
  }
}

void ConstructionView::finishMovingObject()
{
  assert( viewmode_ == MOVEPOLE );

  poleMoveDelegate_->setModelData( poleeditor_, model(), selectionModel()->selectedIndexes().first() );
  delete poleeditor_;
  poleeditor_ = 0;
  viewmode_ = FREEMOVE;
}


void ConstructionView::aboutToRotateObject( GLObject* globject, Vector3D grabPoint )
{
  if( globject 
    && viewobjects_[ globject ] == selectionModel()->selectedIndexes().first() )
    //&& static_cast< PoleData* >( viewobjects_[ globject ].internalPointer() )->pole_->contains( grabPoint ) )
  {
    viewmode_ = ROTATEPOLE;
    assert( !poleeditor_ );
    poleeditor_ = poleRotateDelegate_->createEditor( 0, QStyleOptionViewItem(), viewobjects_[ globject ] );
    poleRotateDelegate_->setEditorData( poleeditor_, viewobjects_[ globject ] );
    poleRotateDelegate_->setEditorMouse( poleeditor_, grabPoint );
    connect( glwidget_, SIGNAL( mouseMoved( QPoint, MyGLWidget::GLMode ) ), poleeditor_, SLOT( updateEvent( QPoint, MyGLWidget::GLMode ) ) );
  }
}


void ConstructionView::finishRotatingObject()
{
  assert( viewmode_ == ROTATEPOLE );

  poleRotateDelegate_->setModelData( poleeditor_, model(), selectionModel()->selectedIndexes().first() );
  delete poleeditor_;
  poleeditor_ = 0;
  viewmode_ = FREEMOVE;
}


void ConstructionView::dataChanged ( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
  //qDebug() << "datachanged";
  assert( topLeft == bottomRight );
  PoleData* poledata = static_cast< PoleData* >( topLeft.internalPointer() );
  qDebug() << topLeft << poledata->pole_->center().toString();
  glwidget_->overwriteObject( viewobjects_.key( topLeft.sibling( topLeft.row(), 0 ) ), GLPoleFactory( *poledata->pole_, poledata->color_ ) );
  glwidget_->updateGL();
}

