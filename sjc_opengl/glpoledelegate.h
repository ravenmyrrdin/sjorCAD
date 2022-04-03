#ifndef GLPOLEDELEGATE_H
#define GLPOLEDELEGATE_H

#include "dllspecopengl.h"
#include "vector3D.h"
#include "sphere3d.h"
#include <QAbstractItemDelegate>
#include "myglwidget.h"

class MyGLWidget;
class Pole;
class Poles;
class GLObject;

class GLPoleEditor : public QObject
{
  Q_OBJECT
  public:
    GLPoleEditor( QObject * parent = 0 ) : QObject( parent ) {}
    virtual ~GLPoleEditor(){}
    virtual void  setInitData( Pole* data, const Poles* model ) = 0;
    virtual void  setInitMouse( const Vector3D& grabPoint ) = 0;
    virtual Pole* getEditedData() const = 0;

  public slots:
    virtual void updateEvent( QPoint mousePos, MyGLWidget::GLMode mode ) = 0;
};

class DLL_EXPORT_OPENGL GLPoleMoveEditor : public GLPoleEditor
{
  Q_OBJECT
  public:
    GLPoleMoveEditor( MyGLWidget* glwidget, QObject* parent = 0 );
    ~GLPoleMoveEditor();
    virtual void  setInitData( Pole* data, const Poles* model );
    virtual void  setInitMouse( const Vector3D& grabPoint );
    virtual Pole* getEditedData() const;

  public slots:
    virtual void updateEvent( QPoint mousePos, MyGLWidget::GLMode mode );

  private:
    Pole collide( const Pole& destinationPole ) const;

    MyGLWidget* glwidget_;
    Pole* destinationPole_;
    Vector3D absGrabPoint_;
    Vector3D relGrabPoint_;
    Vector3D absBeginPos_;

    const GLObject* cylinder_;
    const GLObject* line_;
    const GLObject* destPole_;
    const Poles* model_;
};

class DLL_EXPORT_OPENGL GLPoleRotateEditor : public GLPoleEditor
{
  Q_OBJECT
  public:
    GLPoleRotateEditor( MyGLWidget* glwidget, QObject* parent = 0 );
    ~GLPoleRotateEditor();
    virtual void  setInitData( Pole* data, const Poles* model );
    virtual void  setInitMouse( const Vector3D& grabPoint );
    virtual Pole* getEditedData() const;

  public slots:
    virtual void updateEvent( QPoint mousePos, MyGLWidget::GLMode mode );

  private:
    Pole collide( const Pole& destinationPole ) const;

    MyGLWidget* glwidget_;
    Pole originalPole_;
    Pole destinationPole_;
    Vector3D absGrabPoint_;
    Vector3D relGrabPoint_;
    Vector3D absBeginPos_;
    Sphere3D sphere_;

    const GLObject* glsphere_;
    //const GLObject* line_;
    const GLObject* destPole_;
    const Poles* model_;
};


class GLPoleDelegate : public QObject
{
  public:
    GLPoleDelegate( QObject * parent = 0 ) : QObject( parent ) {}
    virtual GLPoleEditor* createEditor ( QObject* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const = 0;
    virtual void          setEditorData( GLPoleEditor* editor, const QModelIndex& index ) const = 0;
    virtual void          setEditorMouse( GLPoleEditor* editor, const Vector3D& grabPoint ) const = 0;
    virtual void          setModelData ( GLPoleEditor* editor, QAbstractItemModel* model, const QModelIndex& index ) const = 0;
};

class DLL_EXPORT_OPENGL GLPoleMoveDelegate : public GLPoleDelegate
{
  public:
    GLPoleMoveDelegate( MyGLWidget* glwidget, QObject * parent = 0 ) : glwidget_( glwidget ), GLPoleDelegate( parent ){}

    virtual GLPoleEditor* createEditor ( QObject* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void          setEditorData( GLPoleEditor* editor, const QModelIndex& index ) const;
    virtual void          setEditorMouse( GLPoleEditor* editor, const Vector3D& grabPoint ) const;
    virtual void          setModelData ( GLPoleEditor* editor, QAbstractItemModel* model, const QModelIndex& index ) const;

  private:
    MyGLWidget* glwidget_;
};

class DLL_EXPORT_OPENGL GLPoleRotateDelegate : public GLPoleDelegate
{
  public:
    GLPoleRotateDelegate( MyGLWidget* glwidget, QObject * parent = 0 ) : glwidget_( glwidget ), GLPoleDelegate( parent ){}

    virtual GLPoleEditor* createEditor ( QObject* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void          setEditorData( GLPoleEditor* editor, const QModelIndex& index ) const;
    virtual void          setEditorMouse( GLPoleEditor* editor, const Vector3D& grabPoint ) const;
    virtual void          setModelData ( GLPoleEditor* editor, QAbstractItemModel* model, const QModelIndex& index ) const;

  private:
    MyGLWidget* glwidget_;
};

#endif