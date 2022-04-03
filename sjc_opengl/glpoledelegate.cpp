#include "glpoledelegate.h"

#include "collision.h"
#include "plane3d.h"
#include "pole.h"
#include "glfactory.h"
#include "myglwidget.h"
#include "construction.h"
#include <QtTest/QtTest>
#include <cmath>

GLPoleMoveEditor::GLPoleMoveEditor( MyGLWidget* glwidget, QObject* parent ) 
: GLPoleEditor( parent ), glwidget_( glwidget )
{
}

GLPoleMoveEditor::~GLPoleMoveEditor()
{
  glwidget_->removeObject( cylinder_ );
  glwidget_->removeObject( destPole_ );
  glwidget_->removeObject( line_ );
}

void GLPoleMoveEditor::setInitData( Pole* data, const Poles* model )
{
  destinationPole_ = new Pole( *data );
  destPole_ = glwidget_->createObject( GLPoleFactory( *destinationPole_, QColor( Qt::white ) ) );
  model_ = model;
}

void  GLPoleMoveEditor::setInitMouse( const Vector3D& grabPoint )
{
  absGrabPoint_ = grabPoint;
  //absGrabPoint_ = destinationPole_->closestPointOnPoleToLine( glwidget_->absEye(), grabPoint );
  absBeginPos_ = absGrabPoint_;
  relGrabPoint_ = absGrabPoint_ - destinationPole_->center();
  cylinder_ = glwidget_->createObject( GLCylinderFactory( Vector3D(), Vector3D(), 0 ) );
  line_ = glwidget_->createObject( GLLineFactory( Vector3D(), Vector3D() ) );
}

Pole* GLPoleMoveEditor::getEditedData() const
{
  return destinationPole_;
}

void GLPoleMoveEditor::updateEvent( QPoint mousePos, MyGLWidget::GLMode mode )
{
  assert( mode == MyGLWidget::MOVEPOLE || mode == MyGLWidget::RAISEPOLE );
  if( mode == MyGLWidget::MOVEPOLE )
    absGrabPoint_ = glwidget_->planeIntersectionUnderPixel( mousePos, absGrabPoint_.z() );
  else
    absGrabPoint_ = glwidget_->verticalPlaneIntersectionUnderPixel( mousePos, absGrabPoint_.x(), absGrabPoint_.y() );
  destinationPole_->setCenter( absGrabPoint_ - relGrabPoint_ );
  *destinationPole_ = collide( *destinationPole_ );
  glwidget_->overwriteObject( destPole_, GLPoleFactory( *destinationPole_, QColor( Qt::white ) ) );

  Vector3D moved = absGrabPoint_ - absBeginPos_;
  Vector3D bottomOfCylinder = Vector3D( absBeginPos_.x(), absBeginPos_.y(), absBeginPos_.z(), Vector3D::POINT3DCART );
  Vector3D topOfCylinder =    Vector3D( absBeginPos_.x(), absBeginPos_.y(), absGrabPoint_.z(), Vector3D::POINT3DCART );
  float moveEditorRadius = Vector3D( moved.x(), moved.y(), 0, Vector3D::POINT3DCART ).length();
  glwidget_->overwriteObject( cylinder_, GLCylinderFactory( bottomOfCylinder, topOfCylinder, moveEditorRadius ) );
  glwidget_->overwriteObject( line_, GLLineFactory( absBeginPos_, absGrabPoint_ ) );
  //qDebug() << destinationPole_->center();
  //qDebug() << destinationPole_->center().toString();
}

Pole GLPoleMoveEditor::collide( const Pole& destinationPole ) const
{
  QList< Collision > collisions = model_->rawCollisions( destinationPole );

  switch( collisions.size() )
  {
    case 0: return destinationPole;
    case 1:
    {
      Pole p = destinationPole;
      p.setCenter( collisions.first().solution().closestPointTo( p.center() ) );
      return p;
    }
    case 2:
    {
      Pole p = destinationPole;
      p.setCenter( collisions.at( 0 ).solution().intersect( collisions.at( 1 ).solution() ).closestPointTo( p.center() ) );
      return p;
    }
    //default: return destinationPole;
  }
}

GLPoleRotateEditor::GLPoleRotateEditor( MyGLWidget* glwidget, QObject* parent ) 
: GLPoleEditor( parent ), glwidget_( glwidget )
{
}

GLPoleRotateEditor::~GLPoleRotateEditor()
{
  glwidget_->removeObject( glsphere_ );
  glwidget_->removeObject( destPole_ );
}

void GLPoleRotateEditor::setInitData( Pole* data, const Poles* model )
{
  destinationPole_ = Pole( *data );
  originalPole_ = destinationPole_;
  destPole_ = glwidget_->createObject( GLPoleFactory( destinationPole_, QColor( Qt::white ) ) );
  model_ = model;
}

void  GLPoleRotateEditor::setInitMouse( const Vector3D& grabPoint )
{
  sphere_ = Sphere3D( destinationPole_.center(), destinationPole_.center().distance( grabPoint ) );
  glsphere_ = glwidget_->createObject( GLSphereFactory( sphere_ ) );
  //absGrabPoint_ = destinationPole_.closestPointOnPoleToLine( glwidget_->absEye(), grabPoint );
  absGrabPoint_ = grabPoint;
  absBeginPos_ = absGrabPoint_;
  relGrabPoint_ = absGrabPoint_ - destinationPole_.center();
  qDebug() << "Grabbing at: " <<  relGrabPoint_.toString();
}

Pole* GLPoleRotateEditor::getEditedData() const
{
  return new Pole( destinationPole_ );
}

void GLPoleRotateEditor::updateEvent( QPoint mousePos, MyGLWidget::GLMode mode )
{
  Vector3D pProjected = glwidget_->pointUnderPixel( mousePos );
  Vector3D absEye = glwidget_->absEye();
  QList< Vector3D > intersection = sphere_.intersect( Line3D( absEye, pProjected ) );

  if( !intersection.size() )
    return;

  absGrabPoint_ = intersection.last();
  Line3D line1( originalPole_.center(), absBeginPos_ );
  Line3D line2( originalPole_.center(), absGrabPoint_ );
  double angle = line1.angle( line2 );
  Line3D axis( originalPole_.center(), originalPole_.center() + line1.unitVector().crossProduct( line2.unitVector() ) );

  destinationPole_ = originalPole_;
  destinationPole_.rotate( axis, angle );
    
  glwidget_->overwriteObject( destPole_, GLPoleFactory( destinationPole_, QColor( Qt::white ) ) );
}

Pole GLPoleRotateEditor::collide( const Pole& destinationPole ) const
{
  QList< Collision > collisions = model_->rawCollisions( destinationPole );

  switch( collisions.size() )
  {
    case 0: return destinationPole;
    case 1:
    {
      //collisions.first().solution( model_->pole( destinationPole.id() ) );
      
      
      //p.setCenter( collisions.first().solution().closestPointTo( p.center() ) );
      //return p;
    }
    case 2:
    {
      Pole p = destinationPole;
      p.setCenter( collisions.at( 0 ).solution().intersect( collisions.at( 1 ).solution() ).closestPointTo( p.center() ) );
      return p;
    }
    //default: return destinationPole;
  }
  return destinationPole;
}



GLPoleEditor* GLPoleMoveDelegate::createEditor ( QObject* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
  return new GLPoleMoveEditor( glwidget_, 0 );
}

void GLPoleMoveDelegate::setEditorData( GLPoleEditor* editor, const QModelIndex& index ) const
{
  editor->setInitData( static_cast< PoleData* >( index.internalPointer() )->pole_, dynamic_cast< const Poles* >( index.model() ) );
}

void GLPoleMoveDelegate::setEditorMouse( GLPoleEditor* editor, const Vector3D& grabPoint ) const
{
  editor->setInitMouse( grabPoint );
}

void GLPoleMoveDelegate::setModelData ( GLPoleEditor* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
  model->setData( index.sibling( index.row(), 1 ), editor->getEditedData()->center() );
}



GLPoleEditor* GLPoleRotateDelegate::createEditor ( QObject* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
  return new GLPoleRotateEditor( glwidget_, 0 );
}

void GLPoleRotateDelegate::setEditorData( GLPoleEditor* editor, const QModelIndex& index ) const
{
  editor->setInitData( static_cast< PoleData* >( index.internalPointer() )->pole_, dynamic_cast< const Poles* >( index.model() ) );
}

void GLPoleRotateDelegate::setEditorMouse( GLPoleEditor* editor, const Vector3D& grabPoint ) const
{
  editor->setInitMouse( grabPoint );
}

void GLPoleRotateDelegate::setModelData ( GLPoleEditor* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
  model->setData( index.sibling( index.row(), 2 ), editor->getEditedData()->direction() );
}


