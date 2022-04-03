#ifndef GLCONSTRUCTIONVIEW_H
#define GLCONSTRUCTIONVIEW_H

#include "dllspecopengl.h"
#include "myglwidget.h"
#include "glpoledelegate.h"
#include <QAbstractItemView>
#include <QPersistentModelIndex>

class DLL_EXPORT_OPENGL ConstructionView : public QAbstractItemView
{
  Q_OBJECT
  public:
    enum ViewMode { FREEMOVE, MOVEPOLE, ROTATEPOLE };
    ConstructionView( MyGLWidget* glwidget, QWidget* parent = 0 );
    virtual QModelIndex  indexAt    ( const QPoint& point ) const;
    virtual void         scrollTo   ( const QModelIndex&, ScrollHint ){}
    virtual QRect        visualRect ( const QModelIndex& ) const { return QRect(); }
    virtual int          horizontalOffset() const { return 0; }
    virtual int          verticalOffset() const { return 0; }
    virtual bool         isIndexHidden(const QModelIndex &) const { return false; }
    virtual QModelIndex  moveCursor ( CursorAction, Qt::KeyboardModifiers ) { return QModelIndex(); }
    virtual void         setSelection ( const QRect&, QItemSelectionModel::SelectionFlags ){}
    virtual QRegion      visualRegionForSelection ( const QItemSelection& ) const{ return QRegion(); }
    virtual void         rowsInserted( const QModelIndex& parent, int start, int end );
    virtual void         dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight );

  private slots:
    void selectObject      ( GLObject* globject );
    void aboutToMoveObject ( GLObject* globject, Vector3D grabPoint );
    void finishMovingObject();
    void aboutToRotateObject( GLObject* globject, Vector3D grabPoint );
    void finishRotatingObject();

  private:
    MyGLWidget* glwidget_;
    QMap< const GLObject*, QPersistentModelIndex > viewobjects_;
    GLPoleMoveDelegate* poleMoveDelegate_;
    GLPoleRotateDelegate* poleRotateDelegate_;

    GLPoleEditor* poleeditor_;
    ViewMode viewmode_;
};

#endif