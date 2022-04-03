

// OS
#include "windows.h"

// OpenGl
#include <gl\gl.h>

// Qt
#include <QApplication>
#include <QHeaderView>
#include <QtTest/QtTest>
#include "modeltest.h"

// SjorCAD
#include "ui_main.h"
#include "pole.h"
#include "construction.h"
#include "glconstructionview.h"
#include "matrix.h"
#include "plane3d.h"

#include <QFontDatabase>


void init(void);
void display(void);

int main ( int argc, char **argv )
{
  Vector3D p1( 1,0,0,Vector3D::POINT3DCART );
  Vector3D p2( 0,1,0,Vector3D::POINT3DCART );
  Vector3D p3( 0,0,1,Vector3D::POINT3DCART );
  Line3D l( Vector3D(0,0,0,Vector3D::POINT3DCART), 
            Vector3D(0,2,2,Vector3D::POINT3DCART) );
  Plane3D p( p1, p2, p3 );
  qDebug() << p.intersect( l ).toString();
  QApplication app( argc, argv );
  QWidget* widget = new QWidget();
  Ui_Form fac;
  fac.setupUi( widget );

  //fac.widget->drawPole( Pole( Vector3D(0,0,0,Vector3D::POINT3DCART), Vector3D( 10,  0,  0, Vector3D::VECTOR3D ), 0.12f ), QColor( Qt::red   ) );
  //fac.widget->drawPole( Pole( Vector3D(0,0,0,Vector3D::POINT3DCART), Vector3D(  0, 10,  0, Vector3D::VECTOR3D ), 0.12f ), QColor( Qt::green ) );
  //fac.widget->drawPole( Pole( Vector3D(0,0,0,Vector3D::POINT3DCART), Vector3D(  0,  0, 10, Vector3D::VECTOR3D ), 0.12f ), QColor( Qt::blue  ) );
  //fac.widget->drawRectangle( QRectF( -5.0, -5.0, 10.0, 10.0 ) );

  Poles* polemodel = new Poles();
  new ModelTest( polemodel );
  ConstructionView* poleview = new ConstructionView( fac.glwidget );

  fac.uiPolesTreeView->setModel( polemodel );
  poleview->setModel( polemodel );
  poleview->setSelectionModel( fac.uiPolesTreeView->selectionModel() );
  fac.uiPolesTreeView->header()->setResizeMode( 0, QHeaderView::ResizeToContents );
  fac.uiPolesTreeView->header()->setResizeMode( 1, QHeaderView::ResizeToContents );
  fac.uiPolesTreeView->header()->setResizeMode( 2, QHeaderView::ResizeToContents );
  fac.uiPolesTreeView->header()->setResizeMode( 3, QHeaderView::ResizeToContents );
  fac.uiPolesTreeView->header()->setResizeMode( 4, QHeaderView::ResizeToContents );
  fac.uiPolesTreeView->header()->setResizeMode( 5, QHeaderView::ResizeToContents );
  fac.uiPolesTreeView->header()->setResizeMode( 6, QHeaderView::Stretch );

  
  polemodel->createPole( Vector3D(5,0,0,Vector3D::POINT3DCART), Vector3D( 10,  0,  0, Vector3D::VECTOR3D ), 0.12f, QColor( Qt::red   ) );
  polemodel->createPole( Vector3D(0,5,0,Vector3D::POINT3DCART), Vector3D( 0,  10,  0, Vector3D::VECTOR3D ), 0.12f, QColor( Qt::green ) );
  polemodel->createPole( Vector3D(0,0,5,Vector3D::POINT3DCART), Vector3D( 0,  0,  10, Vector3D::VECTOR3D ), 0.12f, QColor( Qt::blue  ) );

  widget->show();

  qDebug() << poleview->indexAt( QPoint( 153,  77 ) );
  qDebug() << poleview->indexAt( QPoint( 311,  18 ) );
  qDebug() << poleview->indexAt( QPoint( 189, 165 ) );

  return app.exec();
}

