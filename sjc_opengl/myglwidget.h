#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "dllspecopengl.h"
#include <QtOpenGL/QGLWidget>
#include <QList>
#include <QPoint>
#include "glfactory.h"
#include "globject.h"

class DLL_EXPORT_OPENGL MyGLWidget : public QGLWidget
{
  Q_OBJECT
  public:
    enum GLMode { NONE, EYEROTATE, EYEMOVE, MOVEPOLE, RAISEPOLE, SELECT, ROTATEPOLE };
    MyGLWidget( QWidget* parent = 0 );

    //void drawPole( const Pole& pole, const QColor& color );
    //void drawRectangle( const QRectF& rect );

    virtual void initializeGL();
    virtual void resizeGL( int wiwdth, int height );
    virtual void paintGL();
 
    Vector3D          absEye() const;
    Vector3D          absLookat() const;
    Vector3D          pointUnderPixel( const QPoint& p );
    GLObject*         objectUnderPixel( const QPoint& p );
    Vector3D          planeIntersectionUnderPixel( const QPoint& p, float z );
    Vector3D          verticalPlaneIntersectionUnderPixel( const QPoint& p, float x, float y );

    const GLObject*   createObject   ( const GLFactory& fac );
    void              removeObject   ( const GLObject* globject );
    void              overwriteObject( const GLObject* globject, const GLFactory& fac );

  signals:
    void objectSelected( GLObject* );
    void objectAboutToMove( GLObject*, Vector3D grabPoint );
    void objectMoved();
    void objectAboutToRotate( GLObject*, Vector3D grabPoint );
    void objectRotated();
    void mouseMoved( const QPoint& p, MyGLWidget::GLMode );

  protected:
    virtual void mousePressEvent  ( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent *event );
    virtual void mouseMoveEvent   ( QMouseEvent* event );
    virtual void wheelEvent       ( QWheelEvent* event );

  private:
    void setRho  ( float angle );
    void setPhi  ( float angle );
    void setTheta( float angle );


    QMap< GLuint, GLObject* > objects_;  ///< Container of all GLObjects in this widget, a GLObject's key is its name
    float rhoEye_;
    float phiEye_;
    float thetaEye_;

    Vector3D lookat_;

    GLuint buffer_[ 4000 ];

    QPoint lastPos_;
    GLMode mode_;
};

#endif

